// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_CoolTime_Enemy2.generated.h"

/**
 * 
 */
UCLASS()
class UE5_GFF2024_API UBTT_CoolTime_Enemy2 : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	explicit UBTT_CoolTime_Enemy2(FObjectInitializer const& ObjectInitializer);
	void Init();
	//タスク実行
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	bool initFlg = false;
	float coolTime_sec = 0.0f;

	//*****ブラックボードの変数*****//
	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName Attack;
	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName SpecialAttackFlg;
	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName CoolTime;
};
