// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/2/BTD_HPorNormalAttack_Enemy2.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
//UE_LOG用
#include "Kismet/KismetSystemLibrary.h"

bool UBTD_HPorNormalAttack_Enemy2::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    // AIコントローラーを取得
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return false;
    }

    AActor* OwnerActor = AIController->GetPawn();
    if (!OwnerActor)
    {
        return false;
    }

    // ブラックボードからターゲットアクターを取得
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return false;
    }

    //ブラックボードから変数の取得
    float HP = BlackboardComp->GetValueAsFloat(HPKeyName);
    bool NormalAttack = BlackboardComp->GetValueAsBool(NormalAttackKeyName);

    if (HP >= 50.0f || NormalAttack == true) {
        return true;
    }

	return false;
}
