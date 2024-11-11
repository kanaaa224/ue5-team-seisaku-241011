// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/1/Enemy1Character.h"

#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Game/Player_Cube.h"
#include "Game/Enemy/1/AIC_Enemy1.h"

#include "Math/UnrealMathUtility.h"

#include "Game/Enemy/Commons/PolygonRotationManager.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemy1Character::AEnemy1Character()
{
	/*:::::変数:::::*/
		//体力
	health = 100.0f;

	IsMoving = false;
	IsMoving = true;

	Vector = { 0.,0.,0., };

	//float AngleX = -FMath::Atan2(CubeVertices[11].Z, CubeVertices[11].Y); // x軸回りの回転
	//float AngleZ = FMath::Atan2(CubeVertices[5].X, CubeVertices[5].Y);    // z軸回りの回転
	//float AngleY = 0.0f; // 必要ないので 0 に設定

	//// 面0がy軸（地面）に平行になるよう回転
	//float AngleX = FMath::Atan2(CubeVertices[0].Z, CubeVertices[0].Y);  // x軸回りの回転
	//float AngleZ = 0.0f;  // z軸回りの回転を固定
	//float AngleY = 0.0f;  // y軸回りの回転は不要

	//// 面0（例えば {0, 11, 5}）がy軸に平行になるよう回転
	//FVector VertexA = CubeVertices[0];
	//FVector VertexB = CubeVertices[11];
	//FVector VertexC = CubeVertices[5];

	//// y軸回りの回転角度を計算
	//float AngleY = 0;//-FMath::Atan2(VertexA.Z, VertexA.X);

	//// x軸回りの回転角度を計算（面のz軸成分がy軸に水平になるように）
	//float AngleX = 0;//FMath::Atan2(VertexA.Z, VertexA.Y);

	//// z軸回りの回転は不要または特定の調整で設定
	//float AngleZ = 0.0f;

	//TArray<FVector> RotatedVertices;
	//for (const auto& Vertex : CubeVertices) {
	//	RotatedVertices.Add(RotateVertex(Vertex, AngleX, AngleY, AngleZ));
	//}
	//RotVertices = RotatedVertices;
	RotationManager = new PolygonRotationManager(CubeVertices, CubeFaces);
	//RotationManager = new PolygonRotationManager(RotatedVertices, CubeFaces);


	/*:::::関数:::::*/
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	// 視野
	PawnSensingComp->SetPeripheralVisionAngle(60.f);
	// 見える範囲
	PawnSensingComp->SightRadius = 2000;
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemy1Character::OnSeePlayer);

	// StaticMeshComponentを追加
	box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// StaticMeshをLaodしてStaticMeshComponentのStaticMeshに設定する
	UStaticMesh* TmpMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LevelPrototyping/Meshes/SM_ChamferCube"));
	box->SetStaticMesh(TmpMesh);

	box->SetupAttachment(RootComponent);

	
	Speed = 0;
}

// Called when the game starts or when spawned
void AEnemy1Character::BeginPlay()
{
	Super::BeginPlay();

	//SpawnDefaultController();

	//GetCharacterMovement()->MaxWalkSpeed = 1200.0f;

	GetCharacterMovement()->MaxAcceleration = 4096.0f; // 高速移動向けに加速を強化
	GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f; // 減速時の制御

	//TargetLocation = new FVector(-1, -1, -1);
}

void AEnemy1Character::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	delete RotationManager;
	//delete TargetLocation;
}

//void AEnemy1Character::BeginDestroy()
//{
	///delete RotationManager;
//}

// Called every frame
void AEnemy1Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Delta = DeltaTime;

	if (TargetLocation.Z > -10000)
	//if (TargetLocation != OldTargetLocation)
	{
		FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, Delta, Speed);
		SetActorLocation(NewLocation);
		//UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(TargetLocation.Z), true, true, FColor::Blue, 2.f);

	}

	OldTargetLocation = TargetLocation;

	if (IsMoving)
	{
		MoveProcess();
	}
	

	//AddMovementInput({ 1,0,0 }, 100);

	//// 面0（例えば {0, 11, 5}）がy軸に平行になるよう回転
	//FVector VertexA = CubeVertices[0];
	//FVector VertexB = CubeVertices[11];
	//FVector VertexC = CubeVertices[5];

	//a += 0.001;

	//// y軸回りの回転角度を計算
	//float AngleY = 0 + a;//-FMath::Atan2(VertexA.Z, VertexA.X);

	//// x軸回りの回転角度を計算（面のz軸成分がy軸に水平になるように）
	//float AngleX = 0;//FMath::Atan2(VertexA.Z, VertexA.Y);

	//// z軸回りの回転は不要または特定の調整で設定
	//float AngleZ = 0.0f;

	//TArray<FVector> RotatedVertices;
	//for (const auto& Vertex : CubeVertices) {
	//	RotatedVertices.Add(RotateVertex(Vertex, AngleX, AngleY, AngleZ));
	//}
	//RotVertices = RotatedVertices;
}

void AEnemy1Character::OnSeePlayer(APawn* Pawn)
{
	AAIC_Enemy1* AIController = Cast<AAIC_Enemy1>(GetController());
	// プレイヤー
	APlayer_Cube* player = Cast<APlayer_Cube>(Pawn);

	if (AIController && player)
	{
		// AIControllerにプレイヤー情報を設定
		AIController->SetPlayerKey(player);
	}

	// 視野に入ったら画面に"See"と表示
	//UKismetSystemLibrary::PrintString(this, "See", true, true, FColor::Blue, 2.f);
}

float AEnemy1Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}

void AEnemy1Character::MoveProcess()
{
	//スケールと位置を指定して面の中心座標を計算・デバッグ描画
	FVector Scale = FVector(100, 100, 100);  // スケール
	FVector Position = GetActorLocation();  // オブジェクトの位置

	//Cube->SetNextBottom(FVector(0, 1, 0), Scale, Position);
	//RotationManager->SetNextBottom(FVector(1, 1, 0), Scale, Position);
	RotationManager->SetNextBottom(Vector, Scale, Position);

	RotationManager->SetNewRotationAndLocation(Position);

	FRotator rot1 = RotationManager->GetNowRotation();

	//box->SetRelativeRotation(-1 * rot1);

	//SetActorRotation(-1 * rot1);

	AddActorWorldRotation(-1 * rot1);
	if (GetActorRotation().Pitch > 360.)
	{
		SetActorRotation({ GetActorRotation().Pitch - 360, GetActorRotation().Yaw,GetActorRotation().Roll });
	}
	if (GetActorRotation().Yaw > 360.)
	{
		SetActorRotation({ GetActorRotation().Pitch, GetActorRotation().Yaw - 360,GetActorRotation().Roll });
	}
	if (GetActorRotation().Roll > 360.)
	{
		SetActorRotation({ GetActorRotation().Pitch, GetActorRotation().Yaw,GetActorRotation().Roll - 360 });
	}


	SetActorLocation(RotationManager->GetNewLocation());


	//デバッグ表示
	//RotationManager->DrawPolyhedronFaceCenters(GetWorld(), *RotationManager, Scale, Position);

	//for (int i = 0; i < CubeFaces.Num(); i++)
	//{
	//	for (int j = 0; j < CubeFaces[i].Num(); j++)
	//	{
	//		int k = j + 1;
	//		if (k >= 3)
	//		{
	//			k = 0;
	//		}
	//		/*FVector Start = RotVertices[CubeFaces[i][j]] * Scale + Position;
	//		FVector End = RotVertices[CubeFaces[i][k]] * Scale + Position;*/
	//		FVector Start = CubeVertices[CubeFaces[i][j]] * Scale + Position;
	//		FVector End = CubeVertices[CubeFaces[i][k]] * Scale + Position;
	//		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.1f);
	//	}
	//}
}



