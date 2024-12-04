// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/2/BTT_CoolTime_Enemy2.h"
#include "BehaviorTree/BlackboardComponent.h"

#define _COOL_TIME_ 2.2f

UBTT_CoolTime_Enemy2::UBTT_CoolTime_Enemy2(FObjectInitializer const& ObjectInitializer)
{
	Attack = "Attack";
	SpecialAttackFlg = "SpecialAttackFlg";
	CoolTime = "CoolTime";
}

void UBTT_CoolTime_Enemy2::Init()
{
	coolTime_sec = 0.0f;
}

EBTNodeResult::Type UBTT_CoolTime_Enemy2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Log, TEXT("ExecuteTask : CoolTime"));

	if (initFlg == false) {
		Init();
		initFlg = true;
	}

	//BlackboardのComponentを変数に代入
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	coolTime_sec += GetWorld()->GetDeltaSeconds();

	if (coolTime_sec >= _COOL_TIME_) {
		UE_LOG(LogTemp, Log, TEXT("CoolTime---------->END %f"),coolTime_sec);
		ensure(BlackboardComp);
		BlackboardComp->SetValueAsBool(Attack, false);
		ensure(BlackboardComp);
		BlackboardComp->SetValueAsBool(SpecialAttackFlg, true);
		ensure(BlackboardComp);
		BlackboardComp->SetValueAsBool(CoolTime, false);
		initFlg = false;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("CoolTime---------->NOW %f"),coolTime_sec);
	}

	return EBTNodeResult::Succeeded;
}
