// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_NormalAttack_Enemy2.generated.h"

#define ATTACK_SPEED 2

/**
 * 
 */
UCLASS()
class UE5_GFF2024_API UBTT_NormalAttack_Enemy2 : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

private:
	//攻撃が終わったか
	bool lifting;
	//攻撃が始まったか
	bool startAttack;

	FRotator CurrentRotaton;
	
public:
	explicit UBTT_NormalAttack_Enemy2(FObjectInitializer const& ObjectInitializer);
	//タスク実行
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	//:::::Blackboard::::://

	//Attack(プレイヤーまでの距離を入れる変数)
	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName AttackKeyName;
};
