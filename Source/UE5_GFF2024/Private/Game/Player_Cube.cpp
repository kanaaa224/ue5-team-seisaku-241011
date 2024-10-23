// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Player_Cube.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
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
#include "Components/TimelineComponent.h"

#define	BLINK_COOLTIME	90

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
	UStaticMesh* mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube"));
	Cube->SetStaticMesh(mesh);

	//マテリアルを追加する
	UMaterial* material = LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial"));
	Cube->SetMaterial(0, material);

	//CollisionPresetを「PhysicsActor」に変更する
	Cube->SetCollisionProfileName(TEXT("PhysicsActor"));

	//HitEventを有効にする
	Cube->BodyInstance.bNotifyRigidBodyCollision = true;

	//camera boomを追加する
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;			//SpringArmの長さを調整する
	CameraBoom->bUsePawnControlRotation = true; 	//PawnのControllerRotationを使用する

	//follow cameraを追加する
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; 	//PawnのControllerRotationを使用する

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

	BlinkCoolTime = 0;

	Timer = 0.f;
	Health = 100.f;

	BlinkFlg = false;
	AttackFlg = false;
	InvincibleFlg = false;
}

// Called when the game starts or when spawned
void APlayer_Cube::BeginPlay()
{
	Super::BeginPlay();

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
	if (!InvincibleFlg)
	{
		UKismetSystemLibrary::PrintString(this, TEXT("damage"));
	}
}

// Called every frame
void APlayer_Cube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timer += DeltaTime;

	if (BlinkCoolTime > 0)
	{
		BlinkCoolTime--;
	}
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
	}
}

void APlayer_Cube::BlinkTimelineUpdate(float Value)
{
	//前方のベクトルを取得
	FVector ForwardVector = GetActorForwardVector();
	//ブリンクの初期座標から前方のベクトルに10かけた値を取得
	FVector NewLocation = (ForwardVector * (Value * 10.f)) + BlinkInitLocation;
	SetActorLocation(NewLocation, true);
}

void APlayer_Cube::AttackTimelineUpdate(float Value)
{
	FRotator ActorRotarion = GetActorRotation();
	SetActorRotation(ActorRotarion + FRotator(0.f, Value * 90.f, 0.f));
}

void APlayer_Cube::BlinkTimelineFinished()
{
	BlinkFlg = false;
	InvincibleFlg = false;
}

void APlayer_Cube::AttackTimelineFinished()
{
	AttackFlg = false;
	InvincibleFlg = false;
}

void APlayer_Cube::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr && !BlinkFlg)
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

	if (Controller != nullptr)
	{
		//ヨーとピッチの入力を加える
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayer_Cube::Blink(const FInputActionValue& Value)
{
	//inputのvalueはboolに変換できる
	if (const bool v = Value.Get<bool>() && BlinkTimeline && !BlinkFlg && !AttackFlg && BlinkCoolTime <= 0)
	{
		BlinkInitLocation = GetActorLocation();
		BlinkTimeline->PlayFromStart();
		BlinkCoolTime = BLINK_COOLTIME;
		BlinkFlg = true;
		InvincibleFlg = true;
	}
}

void APlayer_Cube::Attack(const FInputActionValue& Value)
{
	//inputのvalueはboolに変換できる
	if (const bool v = Value.Get<bool>() && AttackTimeline && !AttackFlg && !BlinkFlg)
	{
		AttackTimeline->PlayFromStart();
		AttackFlg = true;
		InvincibleFlg = true;
	}
}

