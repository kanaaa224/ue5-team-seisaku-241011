// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/1/AIC_Enemy1.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AAIC_Enemy1::AAIC_Enemy1(const FObjectInitializer& ObjectInitializer)
{
	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	// 作成したビヘイビアツリーを設定
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("/Game/Game/enemy/1/AI/BT_Enemy1.BT_Enemy1"));
	BehaviorTree = BTFinder.Object;

	PlayerKeyName = "Player";
	StateKeyName = "State";
}

void AAIC_Enemy1::SetPlayerKey(APawn* player)
{
	ensure(BlackboardComp);

	// ブラックボードで作成したPlayerというキーにプレイヤー情報を入れる
	BlackboardComp->SetValueAsObject(PlayerKeyName, player);
}

APlayer_Cube* AAIC_Enemy1::GetPlayerKey()
{
	ensure(BlackboardComp);

	return Cast<APlayer_Cube>(BlackboardComp->GetValueAsObject(PlayerKeyName));
}

void AAIC_Enemy1::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// AIControllerがPawn所持した際にBBとBTを使用
	BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	BehaviorComp->StartTree(*BehaviorTree);
}

void AAIC_Enemy1::OnUnPossess()
{
	Super::OnUnPossess();
	BehaviorComp->StopTree();
}

void AAIC_Enemy1::BeginPlay()
{
	Super::BeginPlay();

	SetState(0);

	//デバッグ
	//APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	//if (PlayerPawn)
	//{
	//	// プレイヤーの位置をログに出力
	//	UE_LOG(LogTemp, Log, TEXT("Player Location: %s"), *PlayerPawn->GetActorLocation().ToString());

	//	// MoveToLocation の実行と結果確認
	//	EPathFollowingRequestResult::Type MoveResult = MoveToLocation(PlayerPawn->GetActorLocation(), 5.0f, true, true, true, false, 0, true);

	//	if (MoveResult == EPathFollowingRequestResult::RequestSuccessful)
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("MoveToLocation request successful"));
	//	}
	//	else if (MoveResult == EPathFollowingRequestResult::AlreadyAtGoal)
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("Already at goal location"));
	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("MoveToLocation request failed"));
	//	}
	//}
}

void AAIC_Enemy1::MoveToSpecifiedLocation(const FVector Location, float Speed)
{
	int32 a = MoveToLocation(Location, Speed, true, true, true, false, 0, true);
	switch (a)
	{
	case EPathFollowingResult::Success:
		UE_LOG(LogTemp, Log, TEXT("移動成功"));
		break;
	case EPathFollowingResult::Blocked:
		UE_LOG(LogTemp, Warning, TEXT("移動が障害物によってブロックされています"));
		break;
	case EPathFollowingResult::OffPath:
		UE_LOG(LogTemp, Warning, TEXT("目標がパスから外れました"));
		break;
	case EPathFollowingResult::Aborted:
		UE_LOG(LogTemp, Warning, TEXT("移動が中断されました"));
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("不明なエラー"));
		break;
	}
}

void AAIC_Enemy1::SetBBDistanceFromPlayer(APlayer_Cube* player)
{
	//プレヤーまでの距離
	float distance = 0.0f;
	//プレヤーの場所
	FVector playerLocation = player->GetActorLocation();
	//自分の場所
	//FVector myLocation = this->GetActorLocation();
	ensure(BlackboardComp);



	//ブラックボードにプレイヤまでの距離をDistanceFromPlayerに設定
	BlackboardComp->SetValueAsFloat(DistanceFromPlayerKeyName, distance);
}

void AAIC_Enemy1::SetState(int32 state)
{
	ensure(BlackboardComp);

	//ブラックボードにプレイヤまでの距離をDistanceFromPlayerに設定
	BlackboardComp->SetValueAsInt(StateKeyName, state);
}


