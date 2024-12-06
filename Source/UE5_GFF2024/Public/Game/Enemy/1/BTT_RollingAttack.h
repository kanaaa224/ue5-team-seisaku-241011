// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_RollingAttack.generated.h"


class UNiagaraSystem;
class UNiagaraComponent;

/**
 * 
 */
UCLASS()
class UE5_GFF2024_API UBTT_RollingAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
    UBTT_RollingAttack();

protected:
    // タスクが実行されるときに呼び出される
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    // 他にブラックボードキーを追加したい場合
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector AdditionalKey;  // 追加のキーが必要であればこちらに宣言

public:
    int32 Count = 0;
    FVector Vector;
    FVector Target = { 0,0,0 };
    FVector TargetLocation = { 0,0,0 };

    bool IsAttacked = false;

    float Speed = 0.f;

    // Niagaraシステムアセット
    UPROPERTY(EditAnywhere, Category = "Niagara")
    UNiagaraSystem* NiagaraEffect;

    // スポーンする位置のオフセット
    UPROPERTY(EditAnywhere, Category = "Niagara")
    FVector LocationOffset = { 0,0,150 };

    UNiagaraComponent* NiagaraComp;

    bool IsSpawnNiagara = false;
	
    /** サウンドアセット */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* SoundToPlay; 

    /** サウンドアセット */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* SoundToPlay2;

    /** 再生位置のオフセット */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    FVector SoundLocationOffset;

    /** 音量 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    float VolumeMultiplier = 1.0f;

    /** ピッチ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    float PitchMultiplier = 1.0f;

    /** 生成されたAudioComponent */
    UPROPERTY()
    UAudioComponent* AudioComponent;

    bool IsPlayedAudio = false;
};
