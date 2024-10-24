// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Player_Box.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/DamageEvents.h"

// Sets default values
APlayer_Box::APlayer_Box()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->InitBoxExtent(FVector(34.f));
	BoxComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	BoxComponent->CanCharacterStepUpOn = ECB_No;
	BoxComponent->SetShouldUpdatePhysicsVolume(true);
	BoxComponent->SetCanEverAffectNavigation(false);
	BoxComponent->bDynamicObstacle = true;

	RootComponent = BoxComponent;

	//スタティックメッシュを追加する
	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	Cube->SetupAttachment(RootComponent);
	UStaticMesh* mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube"));
	Cube->SetStaticMesh(mesh);

	//マテリアルを追加する
	UMaterial* material = LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial"));
	Cube->SetMaterial(0, material);

	//SimulatePhysicsを有効にする
	Cube->SetSimulatePhysics(true);

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
	
	//Arrowを追加する
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	Arrow->SetupAttachment(FollowCamera);
	
	//頭上に移動するようにLocationを設定する
	Arrow->SetRelativeLocation(FVector(400.f, 0.f, 130.f));

	//Arrowを表示されるようにする
	//Arrow->bHiddenInGame = false;

	//FloatingPawnMovementを追加する
	//FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloationgPawnMovement"));

	CharacterMovement = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("CharacterMovement"));
	if (CharacterMovement)
	{
		CharacterMovement->UpdatedComponent = BoxComponent;
	}

	CharacterMovement->bOrientRotationToMovement = true;
	CharacterMovement->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	CharacterMovement->JumpZVelocity = 700.f;
	CharacterMovement->AirControl = 0.35f;
	CharacterMovement->MaxWalkSpeed = 500.f;
	CharacterMovement->MinAnalogWalkSpeed = 20.f;
	CharacterMovement->BrakingDecelerationWalking = 2000.f;

	//IM_Defaultを読みこむ
	DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/ThirdPerson/Input/IMC_Default"));

	//IA_Lookを読みこむ
	BlinkAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_Blink"));
	//IA_Jumpを読み込む
	MoveAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_Move"));
	//IA_Boostを読み込む
	LookAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_Look"));

	Health = 100.f;

}

// Called when the game starts or when spawned
void APlayer_Box::BeginPlay()
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

void APlayer_Box::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	InflictDamage(Other);
}

// Called every frame
void APlayer_Box::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayer_Box::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//アクションをバインドする
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Move
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayer_Box::Move);

		//Look
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayer_Box::Look);

		//Blink
		EnhancedInputComponent->BindAction(BlinkAction, ETriggerEvent::Started, this, &APlayer_Box::Blink);
	}
}

float APlayer_Box::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health = Health - DamageAmount;

	if (Health <= 0)
	{
		Health = 0;
	}

	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Player_Box Health:%f"), Health));

	return Health;
}

void APlayer_Box::InflictDamage(AActor* Other)
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
	}
}

void APlayer_Box::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
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

		if (CharacterMovement)
		{
			CharacterMovement->AddInputVector(ForwardDirection * MovementVector.Y);
			CharacterMovement->AddInputVector(RightDirection * MovementVector.X);
		}
		//else
		//{
		//	Internal_AddMovementInput(ForwardDirection * MovementVector.Y);
		//	Internal_AddMovementInput(RightDirection * MovementVector.X);
		//}

		//SetActorRotation(UKismetMathLibrary::Conv_VectorToRotator(GetVelocity()));
	}
	//inputのValueはVector2Dに変換できる
	const FVector2D V = Value.Get<FVector2D>();

	//Vectorを計算する
	FVector ForceVector = FVector(V.Y, V.X, 0.f) * 1000.f;

	//Arrowの進行方向のVectorを計算する
	FVector ArrowForceVector = Arrow->GetComponentToWorld().TransformVectorNoScale(ForceVector);

	ArrowForce = ArrowForceVector;

	////Sphereに力を加える
	//Cube->AddForce(ArrowForceVector, TEXT("NONE"), true);

}

void APlayer_Box::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		//ヨーとピッチの入力を加える
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayer_Box::Blink(const FInputActionValue& Value)
{
	//inputのvalueはboolに変換できる
	if (const bool v = Value.Get<bool>())
	{
		////Arrowが向いている前方方向のVector情報を取得する
		//FVector ForwardVector = Arrow->GetForwardVector().GetSafeNormal(0.0001f);

		////torqueとして与えるVectorを作成する
		//FVector TorqueVector = FVector(ForwardVector.Y * 1000 * -1.f, ForwardVector.X * 1000, 0.f);


		//Cube->AddImpulse(ArrowForce * 2, TEXT("None"), true);
		

		////torqueを与えて加速する
		//Cube->AddTorqueInRadians(TorqueVector, TEXT("None"), true);

		FVector ActorLocation = GetActorLocation();
		FVector ActorForwardVector = GetActorForwardVector();

		SetActorLocation(ActorLocation + (ActorForwardVector * 10.f), true);
	}

}

