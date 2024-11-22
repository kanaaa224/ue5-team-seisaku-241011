// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_SetParameter.generated.h"

/**
 * 
 */
UCLASS()
class UE5_GFF2024_API UBTS_SetParameter : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTS_SetParameter();

	// TickNode関数のオーバーライド
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	//:::::Blackboard::::://

	//Distance(プレイヤーまでの距離を入れる変数)
	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName DistanceKeyName;

	//Attack
	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName AttackKeyName;

	//HP
	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName HPKeyName;

	//HPRatio
	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName HPRatioKeyName;
};
