// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Enemy3Move.generated.h"

/**
 * 
 */
UCLASS()
class UE5_GFF2024_API UBTT_Enemy3Move : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UBTT_Enemy3Move();

	//タスク実行
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
