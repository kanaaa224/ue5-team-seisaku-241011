// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/1/BTT_MoveCube.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Game/Enemy/1/Enemy1Character.h"
#include "Game//Enemy/1/AIC_Enemy1.h"
#include "Game/Enemy/Commons/PolygonRotationManager.h"

UBTT_MoveCube::UBTT_MoveCube()
{
	NodeName = "Custom Task";
}

EBTNodeResult::Type UBTT_MoveCube::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // AIコントローラとブラックボードコンポーネントを取得
    AAIController* AIController = OwnerComp.GetAIOwner();
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (AIController == nullptr || BlackboardComp == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    // `Target` キーの値を取得
    UObject* PlayerObject = BlackboardComp->GetValueAsObject(GetSelectedBlackboardKey());

    if (PlayerObject != nullptr)
    {
        // `Target` の値を使った処理を行う
        // 例: ターゲットが見つかった場合の処理など
        UKismetSystemLibrary::PrintString(this, "tes", true, true, FColor::Blue, 2.f);
        
        if (AAIC_Enemy1* AIC = Cast<AAIC_Enemy1>(OwnerComp.GetAIOwner()))
        {
            if (AEnemy1Character* Enemy = Cast<AEnemy1Character>(AIC->GetPawn()))
            {
                if (AActor* Player = Cast<AActor>(PlayerObject))
                {
                    FVector EnemyVector = Player->GetActorLocation() - Enemy->GetActorLocation();

                    FVector Normalize = EnemyVector / EnemyVector.Length();
                    Normalize = { Normalize.X, Normalize.Y, 0. };
                    Enemy->SetVector(Normalize);

                    Enemy->SetIsMoving(true);

                    /*if (!Enemy->IsMoving)
                    {
                        AIC->SetState(1);
                    }*/



                    return EBTNodeResult::Succeeded;

                    if (EnemyVector.Length() < 100)
                    {
                        Enemy->SetIsMoving(false);
                        if (!Enemy->IsMoving)
                        {
                            AIC->SetState(1);
                        }
                        return EBTNodeResult::Succeeded;
                    }
                    else
                    {
                        return EBTNodeResult::Succeeded;
                    }
                }
            }
        }

    }
    else
    {
        // `Target` が設定されていない場合の処理

    }

    // タスク実行後、`Target` に新しい値を設定する例
   // BlackboardComp->SetValueAsObject(GetSelectedBlackboardKey(), nullptr); // 例として `null` に設定

    return EBTNodeResult::Failed;
}
