// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Move_Enemy2.generated.h"

/**
 * 
 */
UCLASS()
class UE5_GFF2024_API UBTT_Move_Enemy2 : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
private:
	FRotator NowRotation;
	bool StopMove;

	FVector Direction;
	FRotator TargetRotation;

	FVector NewLocation;

public:
	explicit UBTT_Move_Enemy2(FObjectInitializer const& ObjectInitializer);
	//タスク実行
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
