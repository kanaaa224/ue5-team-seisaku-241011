// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/Enemy/2/Enemy2AttackObject.h"

//StaticMeshComponent
#include "Components/StaticMeshComponent.h"
//CapsuleComponent
#include "Components/CapsuleComponent.h"
//Movement
#include "GameFramework/CharacterMovementComponent.h"
//PlayController
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy2AttackObject::AEnemy2AttackObject()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//CapsuleSizeを設定
	float CapsuleRadius = 150.0f;  // 半径
	float CapsuleHalfHeight = 320.0f;  // 高さの半分
	GetCapsuleComponent()->SetCapsuleSize(CapsuleRadius, CapsuleHalfHeight);

	//Cubeを作成
	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	CubeMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMeshAsset.Succeeded())
	{
		//メッシュを設定
		CubeMesh->SetStaticMesh(CubeMeshAsset.Object);
		//大きさを設定
		CubeMesh->SetRelativeScale3D(FVector(2.0f, 2.0f, 6.0f));
		//RootComponentに対しての位置を設定
		/*float CubeHeight = 500.0f;
		CubeMesh->SetRelativeLocation(FVector(0.0f, 0.0f, CubeHeight / 2.0f));*/
	}

	//マテリアルをロード
	UMaterialInterface* normalMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Game/enemy/2/Material/M_Normal.M_Normal"));

	if (normalMaterial) {
		CubeMesh->SetMaterial(0, normalMaterial);
	}

	// オーバーラップ開始のイベントをバインド
	CubeMesh->OnComponentBeginOverlap.AddDynamic(this, &AEnemy2AttackObject::OnOverlapBegin);

	//Cube
	CubeMesh->SetCollisionProfileName(UCollisionProfile::CustomCollisionProfileName);
	CubeMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
	CubeMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	CubeMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	CubeMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Overlap);
	//Capsule
	GetCapsuleComponent()->SetCollisionProfileName(UCollisionProfile::CustomCollisionProfileName);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Overlap);
}

// Called when the game starts or when spawned
void AEnemy2AttackObject::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemy2AttackObject::Tick(float DeltaTime)
{
	//初期化
	if (InitFlg == false) {
		//攻撃までの時間を設定
		timeToAttack = createNumber * 5;//数字の部分で攻撃までの時間を変更
		InitFlg = true;
	}

	//生成されて何秒たったか
	Super::Tick(DeltaTime);
	secTime += DeltaTime;

	if (player)
	{
		if (beginAttackFlg == false) {
			RotateTowardsTarget(DeltaTime);
		}
	}
	else
	{
		// プレイヤーを自動的にターゲットにする
		player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	}

	//生成されてから攻撃時間になったら毎フレームこの中の処理をまわす
	if (secTime >= timeToAttack) {
		secAttackTime += DeltaTime;
		beginAttackFlg = true;

		if (secAttackTime <= 2.0f) {
			//プレイヤーの方向に頭を向ける
			HeadToTarget(DeltaTime);
		}
		else {
			if ((player->GetActorLocation().Z + 50.0f) >= GetActorLocation().Z) {
				stopMove = true;
			}

			if (stopMove == false) {
				TargetToMove(DeltaTime);
			}
		}
	}
}

void AEnemy2AttackObject::SetCreateNumber(int createNum)
{
	createNumber = createNum;
}

void AEnemy2AttackObject::RotateTowardsTarget(float deltaTime)
{
	if (!player) return;

	// ターゲットまでの方向を計算
	FVector Direction = player->GetActorLocation() - GetActorLocation();
	Direction.Z = 0.0f;  // Z軸（高さ）は無視して水平に向く
	Direction.Normalize();

	// 現在の向きと目標の向きを補間
	FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	FRotator SmoothRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, deltaTime, 5.0f);

	SetActorRotation(SmoothRotation);
}

void AEnemy2AttackObject::HeadToTarget(float deltaTime)
{
	if (!player) return;

	// ターゲットの位置を取得
	FVector TargetLocation = player->GetActorLocation();
	FVector ActorLocation = GetActorLocation();

	// 方向を計算
	FVector Direction = TargetLocation - ActorLocation;
	Direction.Normalize();

	// 現在の向きと目標の向きを補間
	FRotator CurrentRotation = GetActorRotation();
	FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();

	// プレイヤーの位置を向くため、ピッチ（上下方向）も含める
	TargetRotation.Pitch = FMath::Atan2(Direction.Z, FVector(Direction.X, Direction.Y, 0.0f).Size()) * 180.0f / PI;
	TargetRotation.Pitch += 90.0f;

	FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, deltaTime, 5.0f);

	SetActorRotation(SmoothRotation);
}

void AEnemy2AttackObject::TargetToMove(float deltaTime)
{
	FVector targetLocation = player->GetActorLocation();
	FVector nowLocation = GetActorLocation();

	nowLocation = FMath::VInterpTo(nowLocation, targetLocation, deltaTime, 7.0f);

	SetActorLocation(nowLocation);
}

void AEnemy2AttackObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this) // 自分自身を除外
	{
		UE_LOG(LogTemp, Warning, TEXT("BeginOverlap with Actor: %s"), *OtherActor->GetName());
		UE_LOG(LogTemp, Warning, TEXT("BeginOverlap with Comp: %s"), *OverlappedComp->GetName());
		UE_LOG(LogTemp, Warning, TEXT("BeginOverlap with Comp: %s"), *OtherComp->GetName());
	}
}

