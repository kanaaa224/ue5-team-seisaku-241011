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

	//浮き上がり処理の座標計算を一回のみ
	bool onecCalcFTL_Flg = false;
	//浮き上がりの座標の計算結果
	FVector calcResultFTL;
	//浮き上がりが終わったか
	bool floatEnd = false;

	bool endCreateObject = false;

	//落ちる処理の座標計算を一回のみ
	bool onecCalcDTL_Flg = false;
	//落ちる時の座標の計算結果
	FVector calcResultDTL;
	//落ちる処理が終わったか
	bool downEnd = false;

	//
	float secDownTime = 0.0f;

	//*****ブラックボードの変数*****//
	UPROPERTY(EditDefaultsOnly, Category=AI)
	FName SpecialAttackFlg;
	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName CoolTime;

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

	//地面に降りる
	void Down(AEnemy2Character* myPawn, FVector targetLocation);
	FVector OnecCalcDownTargetLocation(AEnemy2Character* pawn);

	//分身がLevelにいるか調べる関数
	bool AttackObjOfLevel(ULevel* Level, TSubclassOf<AActor> ActorClass);

	//4番目に生成された分身が発射されたか調べる関数
	bool CheckFireFourthAttackObj(UWorld* WorldContext);

private:
	USoundBase* JumpSE;
	// 効果音を再生する関数
	UFUNCTION()
	void PlaySE_Jump(AEnemy2Character* pawn);
	bool onecJumpSE_Flg = false;

	USoundBase* DownSE;
	// 効果音を再生する関数
	UFUNCTION()
	void PlaySE_Down(AEnemy2Character* pawn);
	bool onecDownSE_Flg= false;
};
