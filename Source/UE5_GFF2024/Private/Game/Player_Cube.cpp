// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Player_Cube.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TimelineComponent.h"
#include "Engine/DamageEvents.h"
#include "Game/System/LockOnInterface.h"
#include "Engine/Classes/Particles/ParticleSystem.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Game/System/GameMode_InGame.h"

#define DEFAULT_TARGET_ARM_LENGTH	700.f			//プレイヤーまでのカメラの距離
#define	BLINK_COOLTIME	90							//回避のクールタイム
#define ATTACK_COOLTIME	90							//攻撃のクールタイム

// Sets default values
APlayer_Cube::APlayer_Cube()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(49.f, 49.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1000.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 0.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 900.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	//スタティックメッシュを追加する
	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	Cube->SetupAttachment(RootComponent);
	UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube"));
	Cube->SetStaticMesh(StaticMesh);

	//マテリアルを追加する
	UMaterial* Material = LoadObject<UMaterial>(nullptr, TEXT("/Game/Game/Player/Material/M_Player"));
	//UMaterial* Material = LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial"));

	Cube->SetMaterial(0, Material);
	Material_Instance = Cube->CreateAndSetMaterialInstanceDynamic(0);

	//CollisionPresetを「PhysicsActor」に変更する
	Cube->SetCollisionProfileName(TEXT("PhysicsActor"));

	//HitEventを有効にする
	Cube->BodyInstance.bNotifyRigidBodyCollision = true;

	//camera boomを追加する
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = DEFAULT_TARGET_ARM_LENGTH;			//SpringArmの長さを調整する
	CameraBoom->bUsePawnControlRotation = true; 						//PawnのControllerRotationを使用する
	//CameraBoom->bDoCollisionTest = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(CameraBoom);
	SpringArm->TargetArmLength = 0.f;

	SpringArm->bDoCollisionTest = false;
	//SpringArm->bEnableCameraLag = true;
	//SpringArm->bEnableCameraRotationLag = true;

	//SpringArm->CameraLagSpeed = 5.f;
	//SpringArm->CameraRotationLagSpeed = 5.f;

	//follow cameraを追加する
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;		//PawnのControllerRotationを使用する


	//MotoinBlurをオフにする
	FollowCamera->PostProcessSettings.MotionBlurAmount = 0.f;

	//IM_Defaultを読みこむ
	DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/ThirdPerson/Input/IMC_Default"));

	//IA_Lookを読みこむ
	BlinkAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_Blink"));
	//IA_Jumpを読み込む
	MoveAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_Move"));
	//IA_Boostを読み込む
	LookAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_Look"));
	//IA_Attackを読み込む
	AttackAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_Attack"));
	//IA_LockOnを読み込む
	LockOnAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_LockOn"));

	//BoxComponentを追加する
	LockOnCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	LockOnCollision->SetupAttachment(RootComponent);
	LockOnCollision->SetBoxExtent(FVector(300.f, 300.f, 100.f));
	LockOnCollision->SetCollisionProfileName(UCollisionProfile::CustomCollisionProfileName);					//コリジョンプリセットをカスタムに設定
	LockOnCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);													//コリジョンをクエリーオンリーにする
	LockOnCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);											//コリジョンのオブジェクトタイプをLockOnにする
	LockOnCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);								//コリジョンに対する反応をすべてIgnoreにする
	LockOnCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);		//コリジョンに対する反応をPawnだけOverlapにする

	//LockOnCollision->bHiddenInGame = false;

	//SphereComponentを追加する
	AttackCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	AttackCollision->SetupAttachment(RootComponent);
	AttackCollision->InitSphereRadius(200.f);
	AttackCollision->SetCollisionProfileName(UCollisionProfile::CustomCollisionProfileName);					//コリジョンプリセットをカスタムに設定
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);												//コリジョンを無効にする
	AttackCollision->SetCollisionObjectType(ECollisionChannel::ECC_EngineTraceChannel2);										//コリジョンのオブジェクトタイプをAttackにする
	AttackCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);								//コリジョンに対する反応をすべてIgnoreにする
	AttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);		//コリジョンに対する反応をPawnだけOverlapにする

	//AttackCollision->bHiddenInGame = false;

	AttackParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent0"));
	if (AttackParticle)
	{
		AttackParticleComponent->SetupAttachment(AttackCollision);
		AttackParticleComponent->SecondsBeforeInactive = 1.f;
		AttackParticleComponent->bAutoActivate = false;
	}

	//カーブの作成
	BlinkCurve = NewObject<UCurveFloat>(this, UCurveFloat::StaticClass(), TEXT("BlinkCurve"));
	if (BlinkCurve)
	{
		BlinkCurve->FloatCurve.AddKey(0.f, 0.f);
		BlinkCurve->FloatCurve.AddKey(0.5f, 100.f);
	}
	AttackCurve = NewObject<UCurveFloat>(this, UCurveFloat::StaticClass(), TEXT("AttackCurve"));
	if (AttackCurve)
	{
		AttackCurve->FloatCurve.AddKey(0.f, 0.f);
		AttackCurve->FloatCurve.AddKey(0.5f, 1.f);
	}
	KnockBackCurve = NewObject<UCurveFloat>(this, UCurveFloat::StaticClass(), TEXT("KnockBackCurve"));
	if (KnockBackCurve)
	{
		KnockBackCurve->FloatCurve.AddKey(0.f, 0.f);
		KnockBackCurve->FloatCurve.AddKey(0.5f, 100.f);
	}

	//タイムラインの追加
	BlinkTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("BlinkTimelineComponent"));
	if (BlinkTimeline)
	{
		//タイムライン更新時に呼び出されるメソッドの定義
		FOnTimelineFloat TimelineUpdateFunc;
		TimelineUpdateFunc.BindUFunction(this, TEXT("BlinkTimelineUpdate"));
		BlinkTimeline->AddInterpFloat(BlinkCurve, TimelineUpdateFunc);

		//タイムライン終了時に呼び出されるメソッドの定義
		FOnTimelineEvent TimelineFinishedFunc;
		TimelineFinishedFunc.BindUFunction(this, TEXT("BlinkTimelineFinished"));
		BlinkTimeline->SetTimelineFinishedFunc(TimelineFinishedFunc);
	}
	AttackTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AttackTimelineComponent"));
	if (AttackTimeline)
	{
		//タイムライン更新時に呼び出されるメソッドの定義
		FOnTimelineFloat TimelineUpdateFunc;
		TimelineUpdateFunc.BindUFunction(this, TEXT("AttackTimelineUpdate"));
		AttackTimeline->AddInterpFloat(AttackCurve, TimelineUpdateFunc);

		//タイムライン終了時に呼び出されるメソッドの定義
		FOnTimelineEvent TimelineFinishedFunc;
		TimelineFinishedFunc.BindUFunction(this, TEXT("AttackTimelineFinished"));
		AttackTimeline->SetTimelineFinishedFunc(TimelineFinishedFunc);
	}
	KnockBackTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("KnockBackTimelineComponent"));
	if (KnockBackTimeline)
	{
		//タイムライン更新時に呼び出されるメソッドの定義
		FOnTimelineFloat TimelineUpdateFunc;
		TimelineUpdateFunc.BindUFunction(this, TEXT("KnockBackTimelineUpdate"));
		KnockBackTimeline->AddInterpFloat(KnockBackCurve, TimelineUpdateFunc);

		//タイムライン終了時に呼び出されるメソッドの定義
		FOnTimelineEvent TimelineFinishedFunc;
		TimelineFinishedFunc.BindUFunction(this, TEXT("KnockBackTimelineFinished"));
		KnockBackTimeline->SetTimelineFinishedFunc(TimelineFinishedFunc);
	}

	BlinkInitLocation = FVector(0.f);
	KnockBackInitLocation = FVector(0.f);
	BlinkForwardVector = FVector(0.f);
	KnockBackForwardVector = FVector(0.f);
	CameraImpactPoint = FVector(0.f);

	LockOnTargetActor = nullptr;

	ConstructorHelpers::FObjectFinder<UParticleSystem> FindEff(TEXT("/Game/InfinityBladeEffects/Effects/FX_Mobile/ICE/combat/P_AOE_Ice_CircleAttack"));
	if (FindEff.Succeeded())
	{
		AttackParticle = FindEff.Object;
	}

	BlinkCoolTime = 0;
	AttackCoolTime = 0;

	Timer = 0.f;
	Health = 100.f;

	BlinkFlg = false;
	AttackFlg = false;
	InflictDamageFlg = false;
	InvincibleFlg = false;
	KnockBackFlg = false;
	LockOnFlg = false;
}

// Called when the game starts or when spawned
void APlayer_Cube::BeginPlay()
{
	Super::BeginPlay();

	LockOnCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayer_Cube::OnLockOnCollisionBeginOverlap);
	LockOnCollision->OnComponentEndOverlap.AddDynamic(this, &APlayer_Cube::OnLockOnCollisionEndOverlap);

	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayer_Cube::OnAttackCollisionBeginOverlap);
	AttackCollision->OnComponentEndOverlap.AddDynamic(this, &APlayer_Cube::OnAttackCollisionEndOverlap);

	//InputMappingContextの追加
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void APlayer_Cube::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
}

// Called every frame
void APlayer_Cube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timer += DeltaTime;

	if (BlinkCoolTime > 0)
	{
		BlinkCoolTime--;
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("CT:%d"), BlinkCoolTime), true, true, FColor::Cyan, 0.5f, TEXT("None"));
	}

	if (AttackCoolTime > 0)
	{
		AttackCoolTime--;
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("CT:%d"), AttackCoolTime), true, true, FColor::Cyan, 0.5f, TEXT("None"));
	}

	LockOnTarget();

	SmoothCameraCollision();

	PlayerTransparent();
}

// Called to bind functionality to input
void APlayer_Cube::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//アクションをバインドする
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Move
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayer_Cube::Move);

		//Look
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayer_Cube::Look);

		//Blink
		EnhancedInputComponent->BindAction(BlinkAction, ETriggerEvent::Started, this, &APlayer_Cube::Blink);

		//Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APlayer_Cube::Attack);

		//LockOn
		EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Started, this, &APlayer_Cube::LockOn);
	}
}

float APlayer_Cube::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!InvincibleFlg)
	{
		Health = Health - DamageAmount;
		Health = Health <= 0 ? 0 : Health;
		if (KnockBackTimeline && !KnockBackFlg)
		{
			KnockBackInitLocation = GetActorLocation();
			KnockBackForwardVector = GetActorForwardVector();
			KnockBackTimeline->PlayFromStart();
			KnockBackFlg = true;
			InvincibleFlg = true;
		}
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Player_Cube Health:%f"), Health));

		if (Health <= 0)
		{
			//GameModeを取得して、AGameMode_InGameにCastする
			if (AGameMode_InGame* GameMode = Cast<AGameMode_InGame>(UGameplayStatics::GetGameMode(this->GetWorld())))
			{
				GameMode->KillPlayer(this);
			}
		}
	}

	return Health;
}

void APlayer_Cube::InflictDamage(AActor* Other)
{
	AActor* ImpactActor = Other;
	if ((ImpactActor != nullptr) && (ImpactActor != this))
	{
		//ダメージイベントの作成
		TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);

		//ダメージ量
		const float DamageAmount = 25.0f;
		if (ImpactActor->TakeDamage(DamageAmount, DamageEvent, Controller, this) <= 0.f)
		{
			if (LockOnCandidates.IsValidIndex(0))
			{
				GetCharacterMovement()->bOrientRotationToMovement = true;
				LockOnFlg = false;
				LockOnTargetActor = GetArraySortingFirstElement(LockOnCandidates);
				if (LockOnTargetActor->GetClass()->ImplementsInterface(ULockOnInterface::StaticClass()))
				{
					ILockOnInterface* LockOnInterface = Cast<ILockOnInterface>(LockOnTargetActor);
					LockOnInterface->SetLockOnEnable(false);
				}
				LockOnCandidates.Remove(LockOnTargetActor);
				//消す
				ImpactActor->Destroy();
			}
		}

		//攻撃のダメージフラグを設定
		InflictDamageFlg = true;
		UKismetSystemLibrary::PrintString(this, TEXT("Attack"));
	}
}

void APlayer_Cube::BlinkTimelineUpdate(float Value)
{
	//上方向のベクトルを取得
	FVector UpVector = GetActorUpVector();
	//上方向のベクトルにかける値
	float ZVec = Value < 50 ? Value : 50.f - (Value - 50.f);
	//ブリンクの初期座標から前方のベクトルに10かけた値と上方向のベクトルの値を取得
	FVector NewLocation = (BlinkForwardVector * (Value * 10.f)) + (UpVector * ZVec) + BlinkInitLocation;

	SetActorLocation(NewLocation, true);
}

void APlayer_Cube::AttackTimelineUpdate(float Value)
{
	//回転情報を取得
	FRotator ActorRotarion = GetActorRotation();

	SetActorRelativeRotation(ActorRotarion + FRotator(0.f, Value * 90.f, 0.f));
}

void APlayer_Cube::KnockBackTimelineUpdate(float Value)
{
	//上方向のベクトルを取得
	FVector UpVector = GetActorUpVector();
	//上方向のベクトルにかける値
	float ZVec = Value < 50 ? Value : 50.f - (Value - 50.f);
	//ノックバックの初期座標から前方のベクトルに10かけた値と上方向のベクトルの値を取得
	FVector NewLocation = (KnockBackForwardVector * -(Value * 5.f)) + ((UpVector * ZVec) * 5.f) + KnockBackInitLocation;

	SetActorLocation(NewLocation, true);

	//回転情報を取得
	FRotator ActorRotarion = GetActorRotation();
	float Pitch = Value / 2;

	SetActorRotation(ActorRotarion + FRotator(6.f, 0.f, 0.f));
}

void APlayer_Cube::BlinkTimelineFinished()
{
	BlinkFlg = false;
	InvincibleFlg = false;
}

void APlayer_Cube::AttackTimelineFinished()
{
	AttackFlg = false;
	InflictDamageFlg = false;
	InvincibleFlg = false;
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//AttackParticleComponent->DestroyComponent();
}

void APlayer_Cube::KnockBackTimelineFinished()
{
	KnockBackFlg = false;
	InvincibleFlg = false;
	SetActorRotation(FRotator(0.f, 0.f, 0.f));
}

void APlayer_Cube::OnLockOnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UKismetSystemLibrary::PrintString(this, UKismetSystemLibrary::GetDisplayName(OtherActor));

	LockOnCandidates.AddUnique(OtherActor);

	//ロックオンの候補がいるか調べる
	if (LockOnCandidates.IsValidIndex(0))
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		LockOnFlg = true;
		LockOnTargetActor = GetArraySortingFirstElement(LockOnCandidates);
		if (LockOnTargetActor->GetClass()->ImplementsInterface(ULockOnInterface::StaticClass()))
		{
			ILockOnInterface* LockOnInterface = Cast<ILockOnInterface>(LockOnTargetActor);
			LockOnInterface->SetLockOnEnable(true);
		}
		UKismetSystemLibrary::PrintString(this, TEXT("ON"));
	}

}

void APlayer_Cube::OnLockOnCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//対象のアクターのLockOnInterfaceを無効にする
	//if (OtherActor->GetClass()->ImplementsInterface(ULockOnInterface::StaticClass()))
	//{
	//	GetCharacterMovement()->bOrientRotationToMovement = true;
	//	LockOnFlg = false;
	//	ILockOnInterface* LockOnInterface = Cast<ILockOnInterface>(OtherActor);
	//	LockOnInterface->SetLockOnEnable(false);
	//	UKismetSystemLibrary::PrintString(this, TEXT("OFF"));
	//}

	//LockOnCandidates.Remove(OtherActor);
}

void APlayer_Cube::OnAttackCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UKismetSystemLibrary::PrintString(this, TEXT("AttackCollision:ON"));
	if (AttackFlg && !InflictDamageFlg)
	{
		InflictDamage(OtherActor);
	}
}

void APlayer_Cube::OnAttackCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void APlayer_Cube::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr && !BlinkFlg && !KnockBackFlg && !AttackFlg)
	{
		//進行方向を探す
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//前方向を取得
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		//右方向を取得
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		//動きを加える
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayer_Cube::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr && !LockOnFlg)
	{
		//ヨーとピッチの入力を加える
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);

		////Pawnが持っているControlの角度を取得する
		//FRotator ControlRotate = GetControlRotation();

		////ControllerのPitchの角度を制限する
		//double LimitPitchAngle = FMath::ClampAngle(ControlRotate.Pitch, -40.f, -10.f);

		////PlayerControllerの角度を設定する
		//UGameplayStatics::GetPlayerController(this, 0)->SetControlRotation(FRotator(LimitPitchAngle, ControlRotate.Yaw, ControlRotate.Roll));

	}
}

void APlayer_Cube::Blink(const FInputActionValue& Value)
{
	//inputのvalueはboolに変換できる
	if (const bool v = Value.Get<bool>() &&		//入力されたら
		BlinkTimeline &&						//BlinkTimelineがnullptrではないなら
		!BlinkFlg &&							//ブリンク判定ではないなら
		!AttackFlg &&							//攻撃判定ではないなら
		!KnockBackFlg &&						//ノックバック判定ではないなら
		BlinkCoolTime <= 0)						//ブリンクのクールタイムがないなら
	{
		BlinkInitLocation = GetActorLocation();
		BlinkForwardVector = GetActorForwardVector();
		BlinkTimeline->PlayFromStart();
		BlinkCoolTime = BLINK_COOLTIME;
		BlinkFlg = true;
		InvincibleFlg = true;
	}
}

void APlayer_Cube::Attack(const FInputActionValue& Value)
{
	//inputのvalueはboolに変換できる
	if (const bool v = Value.Get<bool>() &&		//入力されたら
		AttackTimeline &&						//AttackTimelineがnullptrではないなら
		!AttackFlg &&							//攻撃判定なら
		!BlinkFlg &&							//ブリンク判定ではないなら
		!KnockBackFlg &&						//ノックバック判定ではないなら
		AttackCoolTime <= 0)					//攻撃のクールタイムがないなら
	{
		AttackParticleComponent = UGameplayStatics::SpawnEmitterAttached(AttackParticle, RootComponent, NAME_None, FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
		AttackTimeline->PlayFromStart();
		AttackCoolTime = ATTACK_COOLTIME;
		AttackFlg = true;
		InvincibleFlg = true;
		AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void APlayer_Cube::LockOn(const FInputActionValue& Value)
{
	if (!LockOnFlg)
	{
		//ロックオンの候補がいるか調べる
		if (LockOnCandidates.IsValidIndex(0))
		{
			GetCharacterMovement()->bOrientRotationToMovement = false;
			LockOnFlg = true;
			LockOnTargetActor = GetArraySortingFirstElement(LockOnCandidates);
			if (LockOnTargetActor->GetClass()->ImplementsInterface(ULockOnInterface::StaticClass()))
			{
				ILockOnInterface* LockOnInterface = Cast<ILockOnInterface>(LockOnTargetActor);
				LockOnInterface->SetLockOnEnable(true);
			}
			UKismetSystemLibrary::PrintString(this, TEXT("ON"));
		}
	}
	else
	{
		//ロックオンの候補がいるか調べる
		if (LockOnCandidates.IsValidIndex(0))
		{
			GetCharacterMovement()->bOrientRotationToMovement = true;
			LockOnFlg = false;
			LockOnTargetActor = GetArraySortingFirstElement(LockOnCandidates);
			if (LockOnTargetActor->GetClass()->ImplementsInterface(ULockOnInterface::StaticClass()))
			{
				ILockOnInterface* LockOnInterface = Cast<ILockOnInterface>(LockOnTargetActor);
				LockOnInterface->SetLockOnEnable(false);
			}
			UKismetSystemLibrary::PrintString(this, TEXT("OFF"));
		}
	}
}

void APlayer_Cube::SmoothCameraCollision()
{
	if (!LockOnFlg)
	{
		//始点座標
		FVector StartLocation = CameraBoom->GetComponentLocation();
		//カメラの方向ベクトル
		FVector CameraVec = FollowCamera->GetComponentLocation() - StartLocation;
		//正規化
		UKismetMathLibrary::Vector_Normalize(CameraVec, 0.00001);
		//終点座標
		FVector EndLocation = StartLocation + (CameraVec * DEFAULT_TARGET_ARM_LENGTH);
		//半径
		float SphereRadius = 50.f;
		//無視したいアクター
		TArray<AActor*> ActorToIgnore{ this };
		//結果
		FHitResult OutHit;
		//スフィアトレース
		UKismetSystemLibrary::SphereTraceSingle(this, StartLocation, EndLocation, SphereRadius, UEngineTypes::ConvertToTraceType(ECC_Camera), false, ActorToIgnore, EDrawDebugTrace::None, OutHit, true);
		if (OutHit.bBlockingHit)
		{
			CameraImpactPoint = OutHit.ImpactPoint;

			double TargetPoint = UKismetMathLibrary::Vector_Distance(StartLocation, CameraImpactPoint) - SphereRadius;

			CameraBoom->TargetArmLength = UKismetMathLibrary::FInterpTo(CameraBoom->TargetArmLength, TargetPoint, GetWorld()->GetDeltaSeconds(), 3);
		}
		else
		{
			if (!UKismetMathLibrary::NearlyEqual_FloatFloat(CameraBoom->TargetArmLength, DEFAULT_TARGET_ARM_LENGTH, 1.0))
			{
				CameraBoom->TargetArmLength = UKismetMathLibrary::FInterpTo(CameraBoom->TargetArmLength, DEFAULT_TARGET_ARM_LENGTH, GetWorld()->GetDeltaSeconds(), 3);
			}
		}
	}
}

void APlayer_Cube::LockOnTarget()
{
	if (LockOnFlg)
	{
		//向きたい方向へのプレイヤーの回転値を取得
		FRotator FindActorRotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), LockOnTargetActor->GetActorLocation());
		//向きたい方向へのプレイヤーの回転値の補間
		FRotator InterpActorRotarion = UKismetMathLibrary::RInterpTo(this->GetActorRotation(), FindActorRotation, GetWorld()->GetDeltaSeconds(), 10.f);
		SetActorRotation(FRotator(this->GetActorRotation().Pitch, InterpActorRotarion.Yaw, InterpActorRotarion.Roll));
		//向きたい方向へのコントローラーの回転値の補間
		FRotator InterpControlRotation = UKismetMathLibrary::RInterpTo(Controller->GetControlRotation(), FindActorRotation, GetWorld()->GetDeltaSeconds(), 3.f);
		//微調整用
		float Adjustment = 1.5f;
		Controller->SetControlRotation(FRotator(InterpControlRotation.Pitch - Adjustment, InterpControlRotation.Yaw, Controller->GetControlRotation().Roll));
	}
}

void APlayer_Cube::PlayerTransparent()
{
	double Distance = UKismetMathLibrary::Vector_Distance(FollowCamera->GetComponentLocation(), GetActorLocation());
	//Distance:カメラとプレイヤーの距離
	//InRangeA:カメラがこの値までキャラに近づいたら完全にマテリアルを消す
	//InRangeB:カメラがこの値までキャラに近づいたらマテリアルのフェードを開始する
	//OutRangeA:マテリアルが完全に消えるOpacity値
	//OutRnageB:マテリアルが通常状態のOpacity値
	double Opacity = UKismetMathLibrary::MapRangeClamped(Distance, 200, 300, 0, 1);

	Material_Instance->SetScalarParameterValue("Opacity", Opacity);
}

AActor* APlayer_Cube::GetArraySortingFirstElement(TArray<AActor*> Array)
{
	TArray<AActor*> SortArray = Array;

	//最後の配列番号
	int32 LastIndex = SortArray.Num() - 1;

	for (int32 I = 0; I < LastIndex; I++)
	{
		for (int32 J = I + 1; J < LastIndex; J++)
		{
			if (GetDistanceTo(SortArray[I]) > GetDistanceTo(SortArray[J]))
			{
				SortArray.Swap(I, J);
			}
		}
	}

	return SortArray[0];
}
