// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Wait.generated.h"

/**
 * 
 */
UCLASS()
class UE5_GFF2024_API UBTT_Wait : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTT_Wait();

protected:
	// タスクが実行されるときに呼び出される
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
};
