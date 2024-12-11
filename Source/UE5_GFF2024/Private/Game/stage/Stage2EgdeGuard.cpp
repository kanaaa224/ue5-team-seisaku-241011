// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/stage/Stage2EgdeGuard.h"
#include "Game/Player_Cube.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AStage2EgdeGuard::AStage2EgdeGuard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* StaticMeshComponentを作成する */
	StageWall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	/* StaticMeshをLaodしてStaticMeshComponentのStaticMeshに設定する */
	UStaticMesh* LoadMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/EngineMeshes/Cube"));

	RootComponent = StageWall;

	/* StaticMeshをStaticMeshComponentに設定する */
	StageWall->SetStaticMesh(LoadMesh);
	/* StageWallのサイズを変更する */
	StageWall->SetRelativeScale3D(FVector(1.0, 0.25, 2.5));
	/* StageWallの物理挙動を削除 */
	StageWall->SetSimulatePhysics(false);

	/* BoxComponentを作成する */
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	/* RootComponentを子クラスのBoxComponentに設定する */
	HitBox->SetupAttachment(StageWall);
	/* HitBoxのサイズを変更する */
	HitBox->SetRelativeLocation(FVector(0.0, 1163.0, -1.0));
	HitBox->SetRelativeScale3D(FVector(4.0, 35.5, 4.0));
	/* StageWallのCollision用のOnComponentBeginOverlapをBindする */
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AStage2EgdeGuard::OnBoxBeginOverlap);
	/* StageWallのCollision用のOnComponentEndOverlapをBindする */
	HitBox->OnComponentEndOverlap.AddDynamic(this, &AStage2EgdeGuard::OnBoxEndOverlap);


	/* ArrowComponentを作成する */
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	/* RootComponentを子クラスのArrowComponentに設定する */
	Arrow->SetupAttachment(HitBox);
	/* Arrowのサイズを変更する */
	Arrow->SetRelativeTransform(FTransform(FRotator(0.0, 90.0, 90.0), FVector(-0.0, -31.0, 0.25), FVector(0.2485, 0.028, 0.2485)));
	Arrow->SetHiddenInGame(true);

	StageEdgeVector[0] = "Opacity1";
	StageEdgeVector[1] = "Opacity2";
	StageEdgeVector[2] = "Opacity3";
	StageEdgeVector[3] = "Opacity4";

	// 動的マテリアルインスタンスの作成
	// Set Scalar Palameter Value を使う際に必要になる
	

	for (int i = 0; i < 4; i++)
	{
		PlayerIsRange[i] = false;
	}
}

// Called when the game starts or when spawned
void AStage2EgdeGuard::BeginPlay()
{
	Super::BeginPlay();
	
	ApplyMaterialToStageEdge();

	UGameplayStatics::GetAllActorsOfClass(world, actors, FoundActors);

	//for (AActor* Actor : FoundActors)
	//{
	//	if (Actor)
	//	{
	//		// 回転角度（Rotation）を取得
	//		FRotator Rotation = Actor->GetActorRotation();

	//		// ログに出力
	//		UE_LOG(LogTemp, Log, TEXT("Actor: %s, Rotation: %s"), *Actor->GetName(), *Rotation.ToString());
	//	}
	//}

	//DynamicMaterial = StageWall->CreateAndSetMaterialInstanceDynamic(0);  // 0番目のマテリアルインデックスを使用
}

// Called every frame
void AStage2EgdeGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStage2EgdeGuard::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/* 当たったのがPlayerだった場合 */
	if (APlayer_Cube* player = Cast<APlayer_Cube>(OtherActor))
	{
		/* キャストが成功した場合 */
		UE_LOG(LogTemp, Warning, TEXT("Successfully cast OtherActor to AMyCharacter: %s"), *player->GetName());

		FRotator Rotator = GetActorRotation();

		/* 回転をSwitch文の頭として使いたいため 切り上げている */
		int32 result = FMath::CeilToInt(Rotator.Yaw);

		UE_LOG(LogTemp, Warning, TEXT("Rotation Yaw %d "), result);

		switch (result)
		{
		case 0:

			UE_LOG(LogTemp, Warning, TEXT("Change The Material 0"));
			BackApplicableColor();
			PlayerIsRange[0] = true;
			break;

		case 90:

			UE_LOG(LogTemp, Warning, TEXT("Change The Material 90"));
			BackApplicableColor();
			PlayerIsRange[1] = true;
			break;

		case 180:

			UE_LOG(LogTemp, Warning, TEXT("Change The Material 180"));
			BackApplicableColor();
			PlayerIsRange[2] = true;
			break;

		case -90:

			UE_LOG(LogTemp, Warning, TEXT("Change The Material"));
			BackApplicableColor();
			//if (DynamicMaterial)
			//{
			//	//DynamicMaterial->SetScalarParameterValue(StageEdgeVector[3], 1.0f);
			//	//StageWall->SetMaterial(0, DynamicMaterial);
			//}
			PlayerIsRange[3] = true;
			break;

		default:
			break;
		}
	}
	else
	{
		// キャストが失敗した場合
		UE_LOG(LogTemp, Warning, TEXT("Cast failed: OtherActor is not of type AMyCharacter"));
	}
}

void AStage2EgdeGuard::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	/* 当たったのがPlayerだった場合 */
	if (APlayer_Cube* player = Cast<APlayer_Cube>(OtherActor))
	{
		for (int i = 0; i < 4; i++)
		{
			if (PlayerIsRange[i] == true)
			{
				/* オパシティを 0.0に戻し、色を消す */
				//DynamicMaterial->SetScalarParameterValue(StageEdgeVector[i], 0.0f);
				//StageWall->SetMaterial(0, DynamicMaterial);
				StageWall->SetMaterial(0, FrontMaterial);
			}
		}
	}
}

void AStage2EgdeGuard::ApplyMaterialToStageEdge()
{
	/* マテリアルのロード */
	/* MaterialFinderオブジェクトにマテリアルをロードする */
	/* FObjectFinderは特定のアセットをロードするために使う 今回はマテリアルをロードする */
	//static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Game/Game/stage/gamemain/Stage_Material/Stage2Material/M_Stage2transparent1"));
	//if (MaterialFinder.Succeeded())
	//{
	//	UMaterialInterface* BaseMaterial = MaterialFinder.Object;

	//	/* 動的マテリアルの生成 */
	//	DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);

	//	if (DynamicMaterial)
	//	{
	//		/* パラメータを設定 */
	//		DynamicMaterial->SetScalarParameterValue(StageEdgeVector[0], 0.0f);

	//		StageWall->SetMaterial(0, DynamicMaterial);
	//	}
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Base material not found! Check the path."));
	//}

	FrontMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Game/stage/gamemain/Stage_Material/M_transparent"));
	BackMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Game/stage/gamemain/Stage_Material/Material"));
	StageWall->SetMaterial(0, FrontMaterial);
}

void AStage2EgdeGuard::BackApplicableColor()
{
	StageWall->SetMaterial(0, BackMaterial);
}
