// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_FallAttack.generated.h"

class UTimelineComponent;
class UCurveFloat;
//class UNiagaraSystem;
class UParticleSystem;
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



    //// Niagaraシステムアセット
    //UPROPERTY(EditAnywhere, Category = "Niagara")
    //UNiagaraSystem* NiagaraEffect;

    //// スポーンする位置のオフセット
    //UPROPERTY(EditAnywhere, Category = "Niagara")
    //FVector LocationOffset = { 0,0,150 };

    //bool IsSpawnNiagara = false;

    // パーティクルシステムのアセット
    UPROPERTY(EditAnywhere, Category = "Particle")
    UParticleSystem* ParticleSystem;

    // スポーンする位置のオフセット
    UPROPERTY(EditAnywhere, Category = "Particle")
    FVector LocationOffset;

    // スポーンする回転
    UPROPERTY(EditAnywhere, Category = "Particle")
    FRotator RotationOffset;

    // 持続時間を設定
    UPROPERTY(EditAnywhere, Category = "Particle")
    float Duration = 3.f;

    bool IsSpawnParticle = false;


};
