// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Game/Player_Cube.h"
#include "AIC_Enemy1.generated.h"

/**
 * 
 */
UCLASS()
class UE5_GFF2024_API AAIC_Enemy1 : public AAIController
{
	GENERATED_BODY()

public:
	AAIC_Enemy1(const class FObjectInitializer& ObjectInitializer);

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

public:

	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName DistanceFromPlayerKeyName;
	/// <summary>
	/// SetBBDistanceFromPlayer
	/// </summary>
	/// プレヤーまでの距離をブラックボードに設定する
	/// <param name="player"></param>
	void SetBBDistanceFromPlayer(APlayer_Cube* player);
};
