// Fill out your copyright notice in the Description page of Project Settings.
#include "Game/Enemy/2/Enemy2Character.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Game/Player_Cube.h"
#include "Game/Enemy/2/AIC_Enemy2.h"

#include "Math/UnrealMathUtility.h"

#include "Engine/DamageEvents.h"

//ロックオン
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"

//StaticMeshComponent
#include "Components/StaticMeshComponent.h"
//CapsuleComponent
#include "Components/CapsuleComponent.h"
//Movement
#include "GameFramework/CharacterMovementComponent.h"
//Blackboard
#include "BehaviorTree/BlackboardComponent.h"
//PlayerPawnを取得
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
//AttackObject(自分の分身クラス)
#include "Game/Enemy/2/Enemy2AttackObject.h"

// Sets default values
AEnemy2Character::AEnemy2Character()
{
	//AIControllerクラスを設定
	AIControllerClass = AAIC_Enemy2::StaticClass();
	//キャラクターがAIControllerを使うように設定
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	/*:::::変数:::::*/
	//体力
	health = _ENEMY2_MAX_HP_;
	damageMaterialFlg = false;
	timeCnt = 0;

	/*:::::関数:::::*/
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	// 視野
	PawnSensingComp->SetPeripheralVisionAngle(60.f);
	// 見える範囲
	PawnSensingComp->SightRadius = 2000;
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemy2Character::OnSeePlayer);

	//ロックオン//
	//WidgetComponentを追加する
	LockOnMarkerWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	LockOnMarkerWidget->SetupAttachment(RootComponent);
	LockOnMarkerWidget->SetWidgetSpace(EWidgetSpace::Screen);

	//設定したいウィジェットのクラスを作成
	FString LockOnMarkerWidgetPath = TEXT("/Game/Game/UI/WBP_LockOn.WBP_LockOn_C");
	TSubclassOf<UUserWidget>LockOnMarkerWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*LockOnMarkerWidgetPath)).LoadSynchronous();
	//SetWidgetでnullptrを入れないとクラッシュする
	LockOnMarkerWidget->SetWidget(nullptr);
	//WidgetClassを設定
	LockOnMarkerWidget->SetWidgetClass(LockOnMarkerWidgetClass);
	//非表示にする
	LockOnMarkerWidget->SetVisibility(false);

	GetCapsuleComponent()->SetCollisionProfileName(UCollisionProfile::CustomCollisionProfileName);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);

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

	// キャラクターの移動コンポーネントを取得
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent)
	{
		// 重力の影響を無効化
		MovementComponent->GravityScale = 0.0f;
	}

	//当たり判定の設定
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

	// オーバーラップ開始のイベントをバインド
	CubeMesh->OnComponentBeginOverlap.AddDynamic(this, &AEnemy2Character::OnOverlapBegin);
	//オーバーラップ終了のイベントをバインと
	CubeMesh->OnComponentEndOverlap.AddDynamic(this, &AEnemy2Character::OnOverlapEnd);
}
	

// Called when the game starts or when spawned
void AEnemy2Character::BeginPlay()
{
	Super::BeginPlay();
	//マテリアルのセット
	NormalMaterial();

	//スタート時のLocationを取得
	startLocation = GetActorLocation();
}

// Called every frame
void AEnemy2Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (damageMaterialFlg == true) {
		timeCnt += DeltaTime;
		if (timeCnt >= 0.7f) {
			damageMaterialFlg = false;
			NormalMaterial();
		}
	}
}

void AEnemy2Character::OnSeePlayer(APawn* Pawn)
{
	AAIC_Enemy2* AIController = Cast<AAIC_Enemy2>(GetController());
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

void AEnemy2Character::ApplyDamage(AActor* Other)
{
	AActor* ImpactActor = Other;
	if ((ImpactActor != nullptr) && (ImpactActor != this))
	{
		//ダメージイベントの作成
		TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);

		//ダメージ量
		const float DamageAmount = 25.0f;
		ImpactActor->TakeDamage(DamageAmount, DamageEvent, Controller, this);

		UKismetSystemLibrary::PrintString(this, TEXT("Enemy2 : Attack"));
	}
}

float AEnemy2Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//受けたダメージ量分HPを減産
	health -= DamageAmount;
	//ダメージを受けた時に赤くする
	//DamageMaterial();

	//ヘルスがゼロ以下なら死亡処理
	if (health <= 0) {
		//ここに死亡時に実行する処理、関数を書く
		Die();
	}

	//現在のHPを返す
	return health;
}

void AEnemy2Character::Die()
{
	//LockOnを解除
	LockOnMarkerWidget->SetVisibility(false);

	// コリジョンを無効化
	SetActorEnableCollision(false);
	// 一定時間後にオブジェクトを削除
	SetLifeSpan(1.0f); // 1秒後に削除
}

void AEnemy2Character::DamageMaterial()
{
	//マテリアルをロード
	UMaterialInterface* damageMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Game/enemy/2/Material/M_damage.M_damage"));

	if (damageMaterial) {
		CubeMesh->SetMaterial(0, damageMaterial);
		damageMaterialFlg = true;
		UE_LOG(LogTemp, Warning, TEXT("damageMaterial"));
	}
}

void AEnemy2Character::NormalMaterial()
{
	//マテリアルをロード
	UMaterialInterface* normalMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Game/enemy/2/Material/M_Normal.M_Normal"));

	if (normalMaterial) {
		CubeMesh->SetMaterial(0, normalMaterial);
	}
}

void AEnemy2Character::SetLockOnEnable_Implementation(bool LockOnFlg)
{
	if (LockOnFlg)
	{
		LockOnMarkerWidget->SetVisibility(true);
	}
	else
	{
		LockOnMarkerWidget->SetVisibility(false);
	}
}

float AEnemy2Character::GetHP()
{
	return health;
}

void AEnemy2Character::SpawnAttackObject(int createNum)
{

	//スポーンする場所・回転を設定
	FVector SpawnLocation;
	if (createNum == 1 || createNum == 2) {
		SpawnLocation = GetActorLocation() + FVector(0.f, 800.f * createNum, 0.f);
	}
	else if (createNum == 3 || createNum == 4) {
		SpawnLocation = GetActorLocation() + FVector(0.f, -800.f * (createNum - 2), 0.f);
	}
	FRotator SpawnRotation = GetActorRotation();

	//スポーンのパラメータ設定
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; // このキャラクターを所有者に設定
	SpawnParams.Instigator = GetInstigator();

	// `Character` をスポーン
	AEnemy2AttackObject* SpawnedCharacter = GetWorld()->SpawnActor<AEnemy2AttackObject>(AEnemy2AttackObject::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
	if (SpawnedCharacter)
	{
		UE_LOG(LogTemp, Log, TEXT("Pawn successfully spawned! : %d "), createNum);
		SpawnedCharacter->SetCreateNumber(createNum);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn Pawn."));
	}
}

FVector AEnemy2Character::GetStartLocation()
{
	return startLocation;
}

void AEnemy2Character::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this) {
		//UE_LOG(LogTemp, Warning, TEXT("%s has overlapped with %s"), *GetName(), *OtherActor->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Overlap : Begin"));

		//キャラクターのAIコントローラを取得
		AAIController* AIController = Cast<AAIC_Enemy2>(GetController());
		if (AIController) {
			//ブラックボードコンポーネントを取得
			UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
			if (BlackboardComp) {
				//ブラックボードにあるAttack変数の値を取得
				bool AttackFlg = BlackboardComp->GetValueAsBool(TEXT("Attack"));

				//ブラックボードから取得したAttack変数がTrueならプレイヤーに攻撃を与える
				if (AttackFlg == true) {
					AttackPlayer();
				}
			}
		}
	}
}

void AEnemy2Character::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != this) {
		UE_LOG(LogTemp, Warning, TEXT("Overlap : end"));
	}
}

void AEnemy2Character::AttackPlayer()
{
	//プレイヤーのポーンを取得
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn) {
		//ダメージ量を設定または取得
		float DmageAmount = 25.0f;

		//ダメージを与える（最後のパラメータには攻撃元の情報を渡す）
		UGameplayStatics::ApplyDamage(PlayerPawn, DmageAmount, GetController(), this, nullptr);

		//攻撃のサウンドをここに入れる
	}
}
