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

// Sets default values
AEnemy2Character::AEnemy2Character()
{
	//AIControllerクラスを設定
	AIControllerClass = AAIC_Enemy2::StaticClass();
	//キャラクターがAIControllerを使うように設定
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	/*:::::変数:::::*/
	//体力
	health = 100.0f;

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
	float CapsuleRadius = 100.0f;  // 半径
	float CapsuleHalfHeight = 315.0f;  // 高さの半分
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
}

// Called when the game starts or when spawned
void AEnemy2Character::BeginPlay()
{
	Super::BeginPlay();
	
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

	//ヘルスがゼロ以下なら死亡処理
	if (health <= 0) {
		//ここに死亡時に実行する処理、関数を書く
		Die();
	}

	//受けたダメージ量を返す
	return DamageAmount;
}

void AEnemy2Character::Die()
{
	// コリジョンを無効化
	SetActorEnableCollision(false);
	// 一定時間後にオブジェクトを削除
	SetLifeSpan(1.0f); // 1秒後に削除
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

