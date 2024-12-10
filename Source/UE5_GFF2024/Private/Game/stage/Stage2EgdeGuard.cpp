// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/stage/Stage2EgdeGuard.h"
#include "Game/Player_Cube.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMathUtility.h"


// Sets default values
AStage2EgdeGuard::AStage2EgdeGuard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* StaticMeshComponentを作成する */
	StageWall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	/* StaticMeshをLaodしてStaticMeshComponentのStaticMeshに設定する */
	UStaticMesh* LoadMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/EngineMeshes/Cube"));
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
	//DynamicMaterial = StageWall->CreateAndSetMaterialInstanceDynamic(0);  // 0番目のマテリアルインデックスを使用

	for (int i = 0; i < 4; i++)
	{
		PlayerIsRange[i] = false;
	}
}

// Called when the game starts or when spawned
void AStage2EgdeGuard::BeginPlay()
{
	Super::BeginPlay();
	
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

		switch (result)
		{
		case 0:

			if (DynamicMaterial)
			{
				DynamicMaterial->SetScalarParameterValue(StageEdgeVector[0], 1.0f);
			}
			PlayerIsRange[0] = true;
			break;

		case 90:

			if (DynamicMaterial)
			{
				DynamicMaterial->SetScalarParameterValue(StageEdgeVector[1], 1.0f);
			}
			PlayerIsRange[1] = true;
			break;

		case 180:

			if (DynamicMaterial)
			{
				DynamicMaterial->SetScalarParameterValue(StageEdgeVector[2], 1.0f);
			}
			PlayerIsRange[2] = true;
			break;

		case -90:

			if (DynamicMaterial)
			{
				DynamicMaterial->SetScalarParameterValue(StageEdgeVector[3], 1.0f);
			}
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
				DynamicMaterial->SetScalarParameterValue(StageEdgeVector[i], 0.0f);
			}
		}
	}
}

