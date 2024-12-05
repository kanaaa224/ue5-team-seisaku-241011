// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_MoveCube.generated.h"

/**
 * 
 */
UCLASS()
class UE5_GFF2024_API UBTT_MoveCube : public UBTTask_BlackboardBase
{
    GENERATED_BODY()

public:
    UBTT_MoveCube();

protected:
    // タスクが実行されるときに呼び出される
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    // 他にブラックボードキーを追加したい場合
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector AdditionalKey;  // 追加のキーが必要であればこちらに宣言

    bool IsNextState = false;

    bool IsInit = false;

    void Init();


    /** サウンドアセット */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* SoundToPlay;

    /** 再生位置のオフセット */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    FVector SoundLocationOffset;

    /** 音量 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    float VolumeMultiplier = 1.0f;

    /** ピッチ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    float PitchMultiplier = 1.0f;


};
