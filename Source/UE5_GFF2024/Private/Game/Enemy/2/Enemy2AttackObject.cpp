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
//DamageEvent
#include "Engine/DamageEvents.h"

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

	//エフェクト
	// Niagaraコンポーネントの作成
	Fire = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent_Fire"));
	// Niagaraコンポーネントをルートにアタッチ
	Fire->SetupAttachment(GetRootComponent());
	// 初期化時は無効化しておく
	Fire->SetAutoActivate(false);
	// 必要ならエフェクトのアセットを設定（Blueprintで設定可能にする場合はこの行を省略）
	Fire->SetAsset(LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/RocketThrusterExhaustFX/FX/NS_RocketExhaust_Realistic.NS_RocketExhaust_Realistic")));
	//エフェクトの大きさ
	Fire->SetWorldScale3D(FVector(8.0f));
	//エフェクトの角度
	FRotator setRotaton = FRotator::ZeroRotator;
	setRotaton.Pitch += 90.0f;
	Fire->SetWorldRotation(setRotaton);
	//エフェクトの場所
	Fire->SetRelativeLocation(FVector(0.0f, 0.0f, -100.0f));

	//爆発エフェクト
	// Cascade用のParticleSystemコンポーネントの作成
	Bomb = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent_Bomb"));
	// ParticleSystemコンポーネントをルートにアタッチ
	Bomb->SetupAttachment(GetRootComponent());
	// 初期化時は無効化しておく
	Bomb->bAutoActivate = false;
	// エフェクトのアセットを設定
	//Bomb->SetTemplate(LoadObject<UParticleSystem>(nullptr, TEXT("/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Explosion_Big_C.P_Explosion_Big_C")));
	Bomb->SetTemplate(LoadObject<UParticleSystem>(nullptr, TEXT("/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Explosion_Big_B.P_Explosion_Big_B")));
	// エフェクトの場所
	Bomb->SetRelativeLocation(FVector(-50.0f, 0.0f, -500.0f));
	//エフェクトの大きさ
	Bomb->SetWorldScale3D(FVector(8.0f));
}

// Called when the game starts or when spawned
void AEnemy2AttackObject::BeginPlay()
{
	Super::BeginPlay();
	//エフェクトをスポーンする
	SpawnFireEffect();
}

// Called every frame
void AEnemy2AttackObject::Tick(float DeltaTime)
{
	//初期化
	if (InitFlg == false) {
		//攻撃までの時間を設定
		timeToAttack = createNumber * 2;//数字の部分で攻撃までの時間を変更
		InitFlg = true;
	}

	//生成されて何秒たったか
	Super::Tick(DeltaTime);
	secTime += DeltaTime;
	if (player)
	{
		if (beginAttackFlg == false) {
			RotateTowardsTarget(DeltaTime);
			HeadToTarget(DeltaTime);
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
		if (secAttackTime <=0.2f) {
			//プレイヤーの方向に頭を向ける
			HeadToTarget(DeltaTime);
		}
		else {
			//プレイヤーまでの移動をやめる
			if ((player->GetActorLocation().Z + 50.0f) >= GetActorLocation().Z) {
				stopMove = true;
			}

			//プレイヤーまで移動する
			if (stopMove == false) {
				TargetToMove(DeltaTime);
			}
			//地面について自分自身を破棄
			if (stopMove == true) {
				//爆発エフェクトを発生
				if (bombFlg == false) {
					SpawnBombEffect();
					bombFlg = true;
				}
				secDestoryTime += DeltaTime;
				if (secDestoryTime >= 5.0f) {//ここの数字で破棄するまでの時間指定できる
					Destroy();
				}
			}
		}
	}
}

void AEnemy2AttackObject::SetCreateNumber(int createNum)
{
	createNumber = createNum;
}

int AEnemy2AttackObject::GetCreateNumber()
{
	return createNumber;
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

	FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, deltaTime, 100.0f);

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
		//UE_LOG(LogTemp, Warning, TEXT("BeginOverlap with Actor: %s"), *OtherActor->GetName());
		
		APlayer_Cube* tmpPlayer = Cast<APlayer_Cube>(OtherActor);
		if (tmpPlayer) {
			//UE_LOG(LogTemp, Warning, TEXT("Damage Actor ->: %s"), *OtherActor->GetName());

			//プレイヤーのポーンを取得
			APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			if (PlayerPawn) {
				//ダメージ量を設定または取得
				float DmageAmount = 50.0f;

				//ダメージを与える（最後のパラメータには攻撃元の情報を渡す）
				UGameplayStatics::ApplyDamage(PlayerPawn, DmageAmount, GetController(), this, nullptr);

				//攻撃のサウンドをここに入れる
			}
		}
	}
}

void AEnemy2AttackObject::SpawnFireEffect()
{
	Fire->Activate(true);
}

void AEnemy2AttackObject::SpawnBombEffect()
{
	Bomb->Activate(true);
}
