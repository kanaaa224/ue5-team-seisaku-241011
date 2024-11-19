// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/1/BTT_Wait.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Game/Enemy/1/Enemy1Character.h"
#include "Game//Enemy/1/AIC_Enemy1.h"
#include "Game/Enemy/Commons/PolygonRotationManager.h"

UBTT_Wait::UBTT_Wait()
{
}

EBTNodeResult::Type UBTT_Wait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // AIコントローラとブラックボードコンポーネントを取得
    AAIController* AIController = OwnerComp.GetAIOwner();
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (AIController == nullptr || BlackboardComp == nullptr)
    {
        return EBTNodeResult::Failed;
    }


    if (AAIC_Enemy1* AIC = Cast<AAIC_Enemy1>(OwnerComp.GetAIOwner()))
    {
        AIC->SetState(AIC->NextState);
       
       /* if (AEnemy1Character* Enemy = Cast<AEnemy1Character>(AIC->GetPawn()))
        {
            Enemy->GetPolygonRotationManager()->Init();
        }*/
        
        return EBTNodeResult::Succeeded;
    }
    // `Target` キーの値を取得
    UObject* PlayerObject = BlackboardComp->GetValueAsObject(GetSelectedBlackboardKey());

    if (PlayerObject != nullptr)
    {
        // `Target` の値を使った処理を行う
        // 例: ターゲットが見つかった場合の処理など

       

        
    }
    else
    {
        // `Target` が設定されていない場合の処理

    }

    return EBTNodeResult::Failed;
}
