// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/3/AIC_Enemy3.h"
#include "UObject/ConstructorHelpers.h"

AAIC_Enemy3::AAIC_Enemy3(const class FObjectInitializer& ObjectInitializer)
{
	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	// 作成したビヘイビアツリーを設定
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("/Game/Game/enemy/3/AI/BT_Enemy3.BT_Enemy3"));
	BehaviorTree = BTFinder.Object;

	PlayerKeyName = "Player";
}

void AAIC_Enemy3::BeginPlay()
{
	Super::BeginPlay();
}

void AAIC_Enemy3::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// AIControllerがPawn所持した際にBBとBTを使用
	BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	BehaviorComp->StartTree(*BehaviorTree);
}

void AAIC_Enemy3::OnUnPossess()
{
	Super::OnUnPossess();
	BehaviorComp->StopTree();
}

void AAIC_Enemy3::SetPlayerKey(APawn* player)
{
	ensure(BlackboardComp);

	// ブラックボードで作成したPlayerというキーにプレイヤー情報を入れる
	BlackboardComp->SetValueAsObject(PlayerKeyName, player);
}

APlayer_Cube* AAIC_Enemy3::GetPlayerKey()
{
	ensure(BlackboardComp);

	return Cast<APlayer_Cube>(BlackboardComp->GetValueAsObject(PlayerKeyName));
}