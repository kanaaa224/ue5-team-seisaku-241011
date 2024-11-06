// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/1/Enemy1Character_Pawn.h"

#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Game/Player_Cube.h"
#include "Game/Enemy/1/AIC_Enemy1.h"

#include "Math/UnrealMathUtility.h"

#include "Game/Enemy/Commons/PolygonRotationManager.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEnemy1Character_Pawn::AEnemy1Character_Pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*:::::変数:::::*/
		//体力
	health = 100.0f;

	IsMoving = false;
	IsMoving = true;

	Vector = { 0.,0.,0., };

	RotationManager = new PolygonRotationManager(CubeVertices, CubeFaces);

	/*:::::関数:::::*/

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	// 視野
	PawnSensingComp->SetPeripheralVisionAngle(60.f);
	// 見える範囲
	PawnSensingComp->SightRadius = 2000;
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemy1Character_Pawn::OnSeePlayer);

	// StaticMeshComponentを追加し、RootComponentに設定する
	box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// StaticMeshをLaodしてStaticMeshComponentのStaticMeshに設定する
	UStaticMesh* TmpMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LevelPrototyping/Meshes/SM_ChamferCube"));
	box->SetStaticMesh(TmpMesh);

	// ルートコンポーネントとしてカプセルを追加（必須ではないが推奨）
	UCapsuleComponent* CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;

	CharacterMovement = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("CharacterMovementComponent"));
	CharacterMovement->UpdatedComponent = CapsuleComponent; // Movement の基礎となるコンポーネントを指定
}

// Called when the game starts or when spawned
void AEnemy1Character_Pawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy1Character_Pawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	delete RotationManager;
}

// Called every frame
void AEnemy1Character_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsMoving)
	{
		MoveProcess();
	}

}

// Called to bind functionality to input
void AEnemy1Character_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy1Character_Pawn::OnSeePlayer(APawn* Pawn)
{
	AAIC_Enemy1* AIController = Cast<AAIC_Enemy1>(GetController());
	// プレイヤー
	APlayer_Cube* player = Cast<APlayer_Cube>(Pawn);

	if (AIController && player)
	{
		// AIControllerにプレイヤー情報を設定
		AIController->SetPlayerKey(player);
	}

	UKismetSystemLibrary::PrintString(this, "See", true, true, FColor::Blue, 2.f);
}

float AEnemy1Character_Pawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}

void AEnemy1Character_Pawn::MoveProcess()
{
	//スケールと位置を指定して面の中心座標を計算・デバッグ描画
	FVector Scale = FVector(100, 100, 100);  // スケール
	FVector Position = GetActorLocation();  // オブジェクトの位置

	//Cube->SetNextBottom(FVector(0, 1, 0), Scale, Position);
	//RotationManager->SetNextBottom(FVector(1, 1, 0), Scale, Position);
	RotationManager->SetNextBottom(Vector, Scale, Position);

	RotationManager->SetNewRotationAndLocation(Position);
	FRotator rot1 = RotationManager->GetNowRotation();
	RootComponent->SetRelativeRotation(-1 * rot1);
	SetActorLocation(RotationManager->GetNewLocation());
}

