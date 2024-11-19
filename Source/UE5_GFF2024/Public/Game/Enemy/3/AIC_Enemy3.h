// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Game/Player_Cube.h"
#include "AIC_Enemy3.generated.h"

/**
 * 
 */
UCLASS()
class UE5_GFF2024_API AAIC_Enemy3 : public AAIController
{
	GENERATED_BODY()
	
public:

	AAIC_Enemy3(const class FObjectInitializer& ObjectInitializer);

public:

	void SetPlayerKey(APawn* player);

	UFUNCTION()
	APlayer_Cube* GetPlayerKey();

	UPROPERTY()
	UBehaviorTreeComponent* BehaviorComp;

	UPROPERTY()
	UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName PlayerKeyName;

protected:

	// AIControllerのPawn所持
	virtual void OnPossess(class APawn* InPawn) override;

	// AIControllerのPawn所持解除
	virtual void OnUnPossess() override;

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	class UBehaviorTree* BehaviorTree;

	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }
};
