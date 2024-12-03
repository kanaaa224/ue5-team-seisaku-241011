// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/3/Enemy3Character.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Game/Player_Cube.h"
#include "Game/Enemy/3/AIC_Enemy3.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Game/Enemy/3/Enemy3AttackAllCollsion.h"

// Sets default values
AEnemy3Character::AEnemy3Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 重力無し
	UCharacterMovementComponent* Component = GetCharacterMovement();
	if (Component)
	{
		Component->GravityScale = 0.0f;
	}

	// 視野用のコンポーネントを作成
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	// 視野
	PawnSensingComp->SetPeripheralVisionAngle(60.f);
	// 見える範囲
	PawnSensingComp->SightRadius = 2000;
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemy3Character::OnSeePlayer);
	

	/* AIControllerの設定*/
	AIControllerClass = AAIC_Enemy3::StaticClass();
	//キャラクターがAIControllerを使うように設定
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	/* 最初のRootComponent(親Component)をCapsuleComponent*/
	RootComponent = GetCapsuleComponent();
	//CapsuleSizeを設定
	float CapsuleRadius = 100.0f;  // 半径
	float CapsuleHalfHeight = 150.0f;  // 高さの半分 300
	GetCapsuleComponent()->SetCapsuleSize(CapsuleRadius, CapsuleHalfHeight);
	GetCapsuleComponent()->SetSimulatePhysics(false);


	/* StaticMeshComponentを作成する */
	DiceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	/* StaticMeshをLaodしてStaticMeshComponentのStaticMeshに設定する */
	UStaticMesh* LoadMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Game/enemy/3/Mesh/DiceStaticMesh.DiceStaticMesh"));
	/* StaticMeshをStaticMeshComponentに設定する */
	DiceMesh->SetStaticMesh(LoadMesh);
	/* RootComponentを子クラスのStaticMeshに設定する */
	DiceMesh->SetupAttachment(RootComponent);
	/* DiceMeshのサイズを変更する */
	DiceMesh->SetRelativeScale3D(FVector(1.5, 1.5, 1.5));
	DiceMesh->SetSimulatePhysics(false);


	/* BoxComponentを作成する */
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	/* RootComponentを子クラスのBoxComponentに設定する */
	HitBox->SetupAttachment(DiceMesh);
	/* BoxComponentのサイズを変更する */
	HitBox->SetRelativeScale3D(FVector(1.7, 1.7, 1.7));
	/* Enemy本体のCollision用のOnComponentBeginOverlapをBindする */
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy3Character::OnBoxBeginOverlap);
	/* Enemy本体のCollision用のOnComponentEndOverlapをBindする */
	HitBox->OnComponentEndOverlap.AddDynamic(this, &AEnemy3Character::OnBoxEndOverlap);


	/* ビーム攻撃の仮Effectを読み込み*/
	NiagaraEffect = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/RocketThrusterExhaustFX/FX/NS_RocketExhaust_Realistic"));

}

// Called when the game starts or when spawned
void AEnemy3Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy3Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy3Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy3Character::OnSeePlayer(APawn* Pawn)
{
	AAIC_Enemy3* AIController = Cast<AAIC_Enemy3>(GetController());
	// プレイヤー
	APlayer_Cube* player = Cast<APlayer_Cube>(Pawn);

	if (AIController && player)
	{
		// AIControllerにプレイヤー情報を設定
		AIController->SetPlayerKey(player);
	}

	// 視野に入ったら画面に"See"と表示
	UKismetSystemLibrary::PrintString(this, "Player::See", true, true, FColor::White, 2.f);
}

void AEnemy3Character::Attack_Beam_Up()
{
	FVector target = FVector(0.0, 0.0, 1000.0);
	FVector nowLocation = GetActorLocation();

	//UE_LOG(LogTemp, Display, TEXT("Enemy3 z target.Z %lf"), target.Z);
	//UE_LOG(LogTemp, Display, TEXT("Enemy3 z location %lf"), nowLocation.Z);

	//VInterpTo(現時点の位置, 到達地点, 呪文, そこまで何秒で付きたいか)
	nowLocation = FMath::VInterpTo(nowLocation, target, GetWorld()->GetDeltaSeconds(), 5.0f);
	//UE_LOG(LogTemp, Display, TEXT("Enemy3 z End location %lf"), nowLocation.Z);

	SetActorLocation(nowLocation);
}

void AEnemy3Character::Attack_Beam_Effect()
{
	/* スポーン位置のオフセット */
	FTransform SpawnTransform(FRotator(-90.0, -90.0, 0.0), FVector(0.0, 0.0, -100.0), FVector(5.0, 5.0, 8.0));
	FQuat quat = SpawnTransform.GetRotation();

	/* NiagaraEffectをSpawnさせる関数の回転引数がTRotatorなので、合わせるために値を変換 */
	UE::Math::TRotator<double> SpawnRotation(quat.Rotator());

	if (EffectSpawnFlg == false)
	{
		FVector SpawnLocation = GetActorLocation() + SpawnTransform.GetLocation();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation
		(
			GetWorld(),
			NiagaraEffect,
			SpawnLocation,
			SpawnRotation,
			SpawnTransform.GetScale3D()

		);
		//UE_LOG(LogTemp, Display, TEXT("Enemy3 Beam Effect Yes Spawn"));
		//UE_LOG(LogTemp, Display, TEXT("Enemy3 Beam Effect Location.Z %lf"), SpawnLocation.Z);
		EffectSpawnFlg = true;

		Beam_Collision_Spawn();
	}
	else
	{
		//UE_LOG(LogTemp, Display, TEXT("Enemy3 Beam Effect Not Spawn"));
	}
}

void AEnemy3Character::Beam_Collision_Spawn()
{
	FVector SpawnLocation = GetActorLocation();
	SpawnLocation.Z -= 100.0;

	FRotator SpawnRotation = GetActorRotation();
	SpawnRotation.Quaternion();
	FQuat SpawnQuat = SpawnRotation.Quaternion();

	FVector SpawnScale = FVector(0.3, 0.3, 30.0);

	//スポーンのパラメータ設定
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; // このキャラクターを所有者に設定
	SpawnParams.Instigator = GetInstigator();

	AEnemy3AttackAllCollsion* SpawnedCharacter = GetWorld()->SpawnActor<AEnemy3AttackAllCollsion>(AEnemy3AttackAllCollsion::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
	//UE_LOG(LogTemp, Display, TEXT("Enemy3 Beam Collision Yes Spawn"));

	SpawnedCharacter->SetActorScale3D(SpawnScale);
}

void AEnemy3Character::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AEnemy3Character::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}