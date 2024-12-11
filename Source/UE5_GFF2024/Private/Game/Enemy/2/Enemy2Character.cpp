// Fill out your copyright notice in the Description page of Project Settings.
#include "Game/Enemy/2/Enemy2Character.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Game/Player_Cube.h"
#include "Game/Enemy/2/AIC_Enemy2.h"

#include "Math/UnrealMathUtility.h"

//ダメージイベント
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

#include "Game/UI/HUD_PlayerHUD.h" // HPゲージ表示用のHUDクラス

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
	//ダメージを受けて何秒マテリアルを変化させるか
	sec_ChangeDamegeMaterial = 0.0f;

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

	//エフェクト
	NormalSpark = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent_NormalSpark"));
	NormalSpark->SetupAttachment(CubeMesh);
	NormalSpark->bAutoActivate = false;
	NormalSpark->SetTemplate(LoadObject<UParticleSystem>(nullptr, TEXT("/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Sparks/P_Sparks_F.P_Sparks_F")));
	NormalSpark->SetRelativeLocation(FVector(0.0f,0.0f,100.0f));
	NormalSpark->SetWorldRotation(FRotator(90.0f, 0.0f, 0.0f));
	NormalSpark->SetWorldScale3D(FVector(1.0f));

	SpecialSpark = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent_SpecialSpark"));
	SpecialSpark->SetupAttachment(GetRootComponent());
	SpecialSpark->bAutoActivate = false;
	SpecialSpark->SetTemplate(LoadObject<UParticleSystem>(nullptr, TEXT("/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Sparks/P_Sparks_B.P_Sparks_B")));
	SpecialSpark->SetRelativeLocation(FVector(0.0f));
	SpecialSpark->SetWorldScale3D(FVector(100.0f));
}
	

// Called when the game starts or when spawned
void AEnemy2Character::BeginPlay()
{
	Super::BeginPlay();
	//マテリアルのセット
	NormalMaterial();

	//スタート時のLocationを取得
	startLocation = GetActorLocation();

	// HPゲージの更新
	if (true) {
		AHUD_PlayerHUD* hud = Cast<AHUD_PlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());

		if (hud)
		{
			hud->set_enemyName(FText::FromString(TEXT("縦長長方形")));
			hud->set_enemyHP(health);
			hud->set_enemyMaxHP((float)_ENEMY2_MAX_HP_);
			hud->set_isShow_enemyHP(true);
		}
	}
}

// Called every frame
void AEnemy2Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

void AEnemy2Character::Destroyed()
{
	Super::Destroyed();
	UE_LOG(LogTemp, Log, TEXT("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));	
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
	DamageMaterial();

	// HPゲージの更新
	if (true) {
		AHUD_PlayerHUD* hud = Cast<AHUD_PlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());

		if (hud)
		{
			hud->set_enemyHP(health);

			if (health <= 0.0f) hud->set_isShow_enemyHP(false);
		}
	}

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
	

	Destroy();

	//次のLevelに遷移
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			UGameplayStatics::OpenLevel(GetWorld(), FName("Level_TitleMenu"));
		}, 2.f, false
	);  // 2秒後に無効化
	

	UE_LOG(LogTemp, Log, TEXT("Die"));
}

void AEnemy2Character::DamageMaterial()
{
	sec_ChangeDamegeMaterial += 0.3f;
	ChangeDamageMaterial();
	UE_LOG(LogTemp, Log, TEXT("Material---------->damage"));

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			NormalMaterial();//sec_ChangeDamageMaterial秒後に元のマテリアルに変える
			sec_ChangeDamegeMaterial = 0.0f;
			UE_LOG(LogTemp, Log, TEXT("Material---------->normal"));
		}, sec_ChangeDamegeMaterial, false
	); 
}

void AEnemy2Character::ChangeDamageMaterial()
{
	//マテリアルをロード
	UMaterialInterface* damageMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Game/enemy/2/Material/M_damage.M_damage"));
	CubeMesh->SetMaterial(0, damageMaterial);
}

void AEnemy2Character::NormalMaterial()
{
	//マテリアルをロード
	UMaterialInterface* normalMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Game/enemy/2/Material/M_Normal.M_Normal"));

	if (normalMaterial) {
		CubeMesh->SetMaterial(0, normalMaterial);
	}
}

void AEnemy2Character::TrueSpecialSparkEffect()
{
	SpecialSpark->Activate(true);
}

void AEnemy2Character::FalseSpecialSparkEffct()
{
	SpecialSpark->Activate(false);
}

void AEnemy2Character::TrueNormalSparkEffect()
{
	NormalSpark->Activate(true);
}

void AEnemy2Character::FalseNormalSparkEffect()
{
	NormalSpark->Activate(false);
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

				APlayer_Cube* tmpPlayer = Cast<APlayer_Cube>(OtherActor);
				//ブラックボードから取得したAttack変数がTrueならプレイヤーに攻撃を与える
				if (AttackFlg == true && tmpPlayer) {
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
