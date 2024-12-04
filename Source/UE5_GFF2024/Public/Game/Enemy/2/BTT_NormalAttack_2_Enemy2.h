// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

//Enemy2Character
#include "Game/Enemy/2/Enemy2Character.h"

#include "BTT_NormalAttack_2_Enemy2.generated.h"

/**
 * 
 */
UCLASS()
class UE5_GFF2024_API UBTT_NormalAttack_2_Enemy2 : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	explicit UBTT_NormalAttack_2_Enemy2(FObjectInitializer const& ObjectInitializer);

	//タスク実行
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	//*****Fall関数で使う変数*****//
	//一回だけ取得する
	bool doOnec_Get_TargetRotation = false;
	//回転の最終場所
	FRotator targetRotation;
	//倒れる処理が終わったか
	bool endFall = false;

private:
	//倒れる処理
	void Fall(AEnemy2Character* pawn);

};
