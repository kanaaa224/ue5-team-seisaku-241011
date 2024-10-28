// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/2/AIC_Enemy2.h"
#include "UObject/ConstructorHelpers.h"

AAIC_Enemy2::AAIC_Enemy2(const FObjectInitializer& ObjectInitializer)
{
	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	// 作成したビヘイビアツリーを設定
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("/Game/Game/enemy/2/AI/BT_Enemy2.BT_Enemy2"));
	BehaviorTree = BTFinder.Object;

	PlayerKeyName = "Player";
}

void AAIC_Enemy2::BeginPlay()
{
	Super::BeginPlay();
}

void AAIC_Enemy2::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// AIControllerがPawn所持した際にBBとBTを使用
	BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	BehaviorComp->StartTree(*BehaviorTree);
}

void AAIC_Enemy2::OnUnPossess()
{
	Super::OnUnPossess();
	BehaviorComp->StopTree();
}

void AAIC_Enemy2::SetPlayerKey(APawn* player)
{
	ensure(BlackboardComp);

	// ブラックボードで作成したPlayerというキーにプレイヤー情報を入れる
	BlackboardComp->SetValueAsObject(PlayerKeyName, player);
}

APlayer_Cube* AAIC_Enemy2::GetPlayerKey()
{
	ensure(BlackboardComp);

	return Cast<APlayer_Cube>(BlackboardComp->GetValueAsObject(PlayerKeyName));
}
