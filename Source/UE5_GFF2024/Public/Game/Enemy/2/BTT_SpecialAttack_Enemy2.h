// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

//Enemy2のキャラクター
#include "Game/Enemy/2/Enemy2Character.h"

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

	bool onecCalcFTL_Flg = false;
	FVector calcResultFTL;
	bool floatEnd = false;

	bool endCreateObject = false;

public:
	explicit UBTT_SpecialAttack_Enemy2(FObjectInitializer const& ObjectInitializer);

	// タスクが開始された時に呼ばれる
	void Init();
	//タスク実行
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

	//浮き上がり処理
	void Float(AEnemy2Character* myPawn, FVector targetLocation);
	FVector OnecCalcFloatTargetLocation(AEnemy2Character* pawn);

	//攻撃前の分身を生成
	void CreateOtherSelf();

	//分身がプレイヤー目掛けて攻撃
	void Attack();

	//攻撃を終えて地面に降り立つ
	void Down();
};
