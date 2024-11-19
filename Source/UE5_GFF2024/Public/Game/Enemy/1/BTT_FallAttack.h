// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_FallAttack.generated.h"

class UTimelineComponent;
class UCurveFloat;

/**
 * 
 */
UCLASS()
class UE5_GFF2024_API UBTT_FallAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTT_FallAttack();

public:

    // タスクが実行されるときに呼び出される
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    // 他にブラックボードキーを追加したい場合
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector AdditionalKey;  // 追加のキーが必要であればこちらに宣言

    
    // タイムラインコンポーネント
    UTimelineComponent* MyTimelineComponent;
    // カーブ
    UCurveFloat* MyCurve;

    bool IsTimelineStart;

    // タイムライン更新時に呼ばれる処理
    UFUNCTION()
    void TimelineStep(float _Value);

    // タイムライン終了時に呼ばれる処理
    UFUNCTION()
    void TimelineFinished();


    FVector Velocity;
    FVector OldVelocity;

    int32 Count;

    double StartLocationZ;
    bool IsStart;
};
