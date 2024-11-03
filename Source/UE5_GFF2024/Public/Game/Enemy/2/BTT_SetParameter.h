// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTT_SetParameter.generated.h"

/**
 * 
 */
UCLASS()
class UE5_GFF2024_API UBTT_SetParameter : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = AI)
	class UBehaviorTree* BehaviorTree;
	
	UPROPERTY()
	UBehaviorTreeComponent* BehaviorComp;

	//ブラックボード
	UPROPERTY()
	UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName DistanceKeyName;

public:
	explicit UBTT_SetParameter(FObjectInitializer const& ObjectInitializer);
	//タスク実行
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
