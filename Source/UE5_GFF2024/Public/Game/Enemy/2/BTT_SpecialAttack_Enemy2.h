// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_SpecialAttack_Enemy2.generated.h"

/**
 * 
 */
UCLASS()
class UE5_GFF2024_API UBTT_SpecialAttack_Enemy2 : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

private:
	bool initFlg;

	bool stopMove;

public:
	explicit UBTT_SpecialAttack_Enemy2(FObjectInitializer const& ObjectInitializer);

	// タスクが開始された時に呼ばれる
	void Init();
	//タスク実行
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
