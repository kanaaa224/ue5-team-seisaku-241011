// Fill out your copyright notice in the Description page of Project Settings.
#include "Game/Enemy/1/Enemy1Character.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Game/Player_Cube.h"
#include "Game/Enemy/1/AIC_Enemy1.h"
#include "Math/UnrealMathUtility.h"
#include "Game/Enemy/Commons/PolygonRotationManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "Game/UI/HUD_PlayerHUD.h" 
#include "Game/UI/Widget_StageClear.h"
#include "Game/System/GameMode_InGame.h"

AEnemy1Character::AEnemy1Character()
{
	//体力
	health = 100.0f;
	//移動フラグ
	IsMoving = false;
	//移動先
	Vector = { 0.,0.,0., };
	//回転制御
	RotationManager = new PolygonRotationManager(CubeVertices, CubeFaces);

	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	// 視野
	PawnSensingComp->SetPeripheralVisionAngle(180.f);
	// 見える範囲
	PawnSensingComp->SightRadius = 16000;
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemy1Character::OnSeePlayer);

	// StaticMeshComponentを追加
	box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// StaticMeshをLaodしてStaticMeshComponentのStaticMeshに設定する
	UStaticMesh* TmpMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LevelPrototyping/Meshes/SM_ChamferCube"));
	box->SetStaticMesh(TmpMesh);

	box->SetWorldScale3D(FVector(3.0f));

	box->SetupAttachment(RootComponent);

	//重力
	GetCharacterMovement()->GravityScale = 0.0f;

	GetCapsuleComponent()->SetCapsuleHalfHeight(220.f);

	Speed = 0;


	// 必要な数のコリジョンを配列に追加
	for (int32 i = 0; i < 6; i++) // 3つのコリジョンを作成する例
	{
		UBoxComponent* NewCollision = CreateDefaultSubobject<UBoxComponent>(*FString::Printf(TEXT("AttackCollision_%d"), i));
		NewCollision->SetupAttachment(RootComponent);
		NewCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		NewCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy1Character::OnAttackHit);

		switch (i)
		{
		case 0://下
			NewCollision->SetRelativeLocation({ 0,0,-120 });
			NewCollision->SetRelativeScale3D({ 4,4,1 });
			//NewCollision->SetWorldScale3D({ 4,4,1 });
			break;
		case 1://上
			NewCollision->SetRelativeLocation({ 0,0,120 });
			NewCollision->SetRelativeScale3D({ 4,4,1 });
			break;
		case 2://後ろ
			NewCollision->SetRelativeLocation({ -120,0,0 });
			NewCollision->SetRelativeScale3D({ 1,4,4 });
			break;
		case 3://前
			NewCollision->SetRelativeLocation({ 120,0,0 });
			NewCollision->SetRelativeScale3D({ 1,4,4 });
			break;
		case 4://左
			NewCollision->SetRelativeLocation({ 0,-120,0 });
			NewCollision->SetRelativeScale3D({ 4,1,4 });
			break;
		case 5://右
			NewCollision->SetRelativeLocation({ 0,120,0 });
			NewCollision->SetRelativeScale3D({ 4,1,4 });
			break;
		default:
			break;
		}

		AttackCollisions.Add(NewCollision);
	}
	BottomCollisionNumber = 0;

	IsAttacking = false;
	IsAttackCoolTime = false;

	AttackState = 0;


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


	health = 100.f;

	MoveDirection = { 0,0,0 };

	IsDestroy = false;
}

// Called when the game starts or when spawned
void AEnemy1Character::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxAcceleration = 4096.0f; // 高速移動向けに加速を強化
	GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f; // 減速時の制御

	AttackCollisions[0]->SetRelativeScale3D({ 4,4,0.5 });
	AttackCollisions[1]->SetRelativeScale3D({ 4,4,0.5 });
	AttackCollisions[2]->SetRelativeScale3D({ 0.5,4,4 });
	AttackCollisions[3]->SetRelativeScale3D({ 0.5,4,4 });
	AttackCollisions[4]->SetRelativeScale3D({ 4,0.5,4 });
	AttackCollisions[5]->SetRelativeScale3D({ 4,0.5,4 });

	// HPゲージの更新
	if (true) {
		AHUD_PlayerHUD* hud = Cast<AHUD_PlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());

		if (hud)
		{
			hud->set_enemyName(FText::FromString(TEXT("立方体")));
			hud->set_enemyHP(health);
			hud->set_enemyMaxHP((float)100);
			hud->set_isShow_enemyHP(true);
		}
	}

	LockOnMarkerWidget->SetVisibility(true);
}

void AEnemy1Character::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	LockOnMarkerWidget->SetVisibility(false);

	delete RotationManager;
	//delete TargetLocation;
}

void AEnemy1Character::Destroyed()
{
	Super::Destroyed();

	LockOnMarkerWidget->SetVisibility(false);

	//UGameplayStatics::OpenLevelBySoftObjectPtr(this, LoadLevel);

	AGameMode_InGame* GameMode = Cast<AGameMode_InGame>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->StageClear();
	}
}

//void AEnemy1Character::BeginDestroy()
//{
	///delete RotationManager;
//}

// Called every frame
void AEnemy1Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// HPゲージの更新
	if (true) {
		AHUD_PlayerHUD* hud = Cast<AHUD_PlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());

		if (hud)
		{
			hud->set_enemyHP(health);

			if (health <= 0.0f) hud->set_isShow_enemyHP(false);
		}
	}

	if (health > 0)
	{
		Delta = DeltaTime;

		if (TargetLocation.Z > -10000 && !IsMoving)
		{

			FVector StartLocation = GetActorLocation();
			FVector EndLocation = StartLocation + MoveDirection * Speed * DeltaTime * 300;

			// ライントレース用のヒット結果
			FHitResult HitResult;

			// トレースの設定
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this); // 自分自身を無視
			//プレイヤーも無視

			bool bDidHit = GetWorld()->LineTraceSingleByChannel(
				HitResult,
				StartLocation,
				EndLocation,
				ECC_Visibility,
				QueryParams
			);

			FVector NewLocation;

			switch ((int)AttackState)
			{
			case 0:
				break;

			case 1:
				NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, Delta, Speed);
				SetActorLocation(NewLocation, true);
				break;

			case 2:
				if (bDidHit)
				{
					// 障害物にぶつかった場合
					UE_LOG(LogTemp, Warning, TEXT("Blocked by: %s"), *HitResult.GetActor()->GetName());
				}
				else
				{
					// 移動可能なら移動
					NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, Delta, Speed);
					SetActorLocation(NewLocation, true);
				}

				break;
			default:
				break;
			}
		}

		GetBottomNumber();
		

		//動いているか
		if (IsMoving)
		{
			//回転移動
			MoveProcess();
		}

		//攻撃中か
		if (IsAttacking)
		{
			//攻撃
			Attack();
		}
	}
	else
	{
		//一回だけ
		if (!IsDestroy)
		{
			IsDestroy = true;

			//5秒後に破壊
			SetLifeSpan(0.1f);
		}
	}
}

void AEnemy1Character::OnSeePlayer(APawn* Pawn)
{
	//AIController
	AAIC_Enemy1* AIController = Cast<AAIC_Enemy1>(GetController());
	//プレイヤー
	APlayer_Cube* player = Cast<APlayer_Cube>(Pawn);

	if (AIController && player)
	{
		//AIControllerにプレイヤー情報を設定
		AIController->SetPlayerKey(player);
	}
}

void AEnemy1Character::ApplyDamage(AActor* Other)
{
	AActor* ImpactActor = Other;
	if ((ImpactActor != nullptr) && (ImpactActor != this))
	{
		if (AActor* Player = Cast<AActor>(ImpactActor)) 
		{
			//ダメージイベントの作成
			TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
			FDamageEvent DamageEvent(ValidDamageTypeClass);

			//ダメージ量
			const float DamageAmount = 25.0f;
			ImpactActor->TakeDamage(DamageAmount, DamageEvent, Controller, this);
		}
	}
}

float AEnemy1Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	ChangeMaterial(HitMaterial);
	health -= DamageAmount;
	UKismetSystemLibrary::PrintString(this, "TakeDamage", true, true, FColor::Blue, 2.f);
	return health;
}

void AEnemy1Character::MoveProcess()
{
	IsAttacking = true;

	//スケールと位置を指定して面の中心座標を計算・デバッグ描画
	FVector Scale = FVector(150, 150, 150);  // スケール
	FVector Position = GetActorLocation();  // オブジェクトの位置

	RotationManager->SetNextBottom(Vector, Scale, Position);

	RotationManager->SetNewRotationAndLocation(Position);

	FRotator rot1 = RotationManager->GetNowRotation();

	AddActorWorldRotation(-1 * rot1);
	if (GetActorRotation().Pitch > 360.)
	{
		SetActorRotation({ GetActorRotation().Pitch - 360, GetActorRotation().Yaw,GetActorRotation().Roll });
	}
	if (GetActorRotation().Yaw > 360.)
	{
		SetActorRotation({ GetActorRotation().Pitch, GetActorRotation().Yaw - 360,GetActorRotation().Roll });
	}
	if (GetActorRotation().Roll > 360.)
	{
		SetActorRotation({ GetActorRotation().Pitch, GetActorRotation().Yaw,GetActorRotation().Roll - 360 });
	}

	SetActorLocation(RotationManager->GetNewLocation());

	//デバッグ表示
	RotationManager->DrawPolyhedronFaceCenters(GetWorld(), *RotationManager, {200,200,200}, Position);
}

void AEnemy1Character::Attack()
{
	if (AttackState != 2) 
	{
		// コリジョンを有効化して攻撃の範囲を設定
		AttackCollisions[BottomCollisionNumber]->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		AttackCollisions[BottomCollisionNumber]->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);  // キャラクターに対してのみ
		AttackCollisions[BottomCollisionNumber]->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AttackCollisions[BottomCollisionNumber]->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

		//攻撃後コリジョンを無効化
		int32 tmp = BottomCollisionNumber;
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, tmp]()
			{
				if (!IsValid(this))
				{
					return;
				}

				if (!AttackCollisions.IsValidIndex(tmp))
				{
					return;
				}

				AttackCollisions[tmp]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}, 1.f, false);  //1秒後に無効化
	}
	else
	{
		for (int i = 0; i < 6; i++)
		{
			//コリジョンを有効化して攻撃の範囲を設定
			AttackCollisions[i]->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			AttackCollisions[i]->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);  // キャラクターに対してのみ
			AttackCollisions[i]->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			AttackCollisions[i]->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

			//攻撃後にすぐコリジョンを無効化
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, i]()
				{
					if (!IsValid(this))
					{
						return;
					}

					if (!AttackCollisions.IsValidIndex(i))
					{
						return;
					}

					AttackCollisions[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				}, 1.f, false);  //1秒後に無効化
		}
	}
}

void AEnemy1Character::OnAttackHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && !IsAttackCoolTime)
	{
		ApplyDamage(OtherActor);
	}
}

void AEnemy1Character::GetBottomNumber()
{
	//一番下の面を求める
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (i != j && AttackCollisions[i]->GetComponentLocation().Z > AttackCollisions[j]->GetComponentLocation().Z)
			{
				BottomCollisionNumber = j;
			}
		}
	}
}

void AEnemy1Character::SetLockOnEnable_Implementation(bool LockOnFlg)
{
	//ロックオンの表示
	if (LockOnFlg)
	{
		LockOnMarkerWidget->SetVisibility(true);
	}
	else
	{
		LockOnMarkerWidget->SetVisibility(false);
	}
}

void AEnemy1Character::ChangeMaterial(UMaterialInterface* NewMaterial)
{
	if (box && NewMaterial)
	{
		//元のマテリアル
		UMaterialInterface* OldMaterial = box->GetMaterial(0);

		//メッシュのマテリアルを変更
		box->SetMaterial(0, NewMaterial);

		//攻撃ヒット時マテリアルを変更後元に戻す
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, OldMaterial]()
			{
				if (!IsValid(this))//エラー対策
				{
					return;
				}
				box->SetMaterial(0, OldMaterial);
			}, 0.3f, false);  //0.3秒後に無効化
	}
}



