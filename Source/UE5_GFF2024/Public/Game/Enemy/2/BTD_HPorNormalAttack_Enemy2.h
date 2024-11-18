// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTD_HPorNormalAttack_Enemy2.generated.h"

/**
 * 
 */
UCLASS()
class UE5_GFF2024_API UBTD_HPorNormalAttack_Enemy2 : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

private:
	//HP
	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName HPKeyName = "MyHealth";

	//NormalAttack
	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName NormalAttackKeyName = "NormalAttack";

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
