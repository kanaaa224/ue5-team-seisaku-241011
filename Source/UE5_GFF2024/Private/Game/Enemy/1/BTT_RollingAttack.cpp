// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/1/BTT_RollingAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Game/Enemy/1/Enemy1Character.h"
#include "Game//Enemy/1/AIC_Enemy1.h"

UBTT_RollingAttack::UBTT_RollingAttack()
{
	NodeName = "Custom Task";
}

EBTNodeResult::Type UBTT_RollingAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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


                    if (Count++ < 30)
                    {
                        Enemy->SetIsMoving(false);
                        FRotator LookAtRotation = FRotationMatrix::MakeFromX(Player->GetActorLocation() - Enemy->GetActorLocation()).Rotator();
                        Enemy->SetActorRotation({ 0,LookAtRotation.Yaw,0 });
                        Vector = Player->GetActorLocation() - Enemy->GetActorLocation();
                        EBTNodeResult::InProgress;
                    }
                    else
                    {
                        FVector Normalize = Vector / Vector.Length();
                        Normalize = { Normalize.X, Normalize.Y, 0. };
                        Enemy->SetIsMoving(false);
                        //Enemy->SetActorLocation(Enemy->GetActorLocation() + Normalize * 10);
                        //Enemy->AddMovementInput(Normalize, 100.f);
                        //AIC->MoveToSpecifiedLocation(Enemy->GetActorLocation() + Normalize * 3, 10.f);
                        AIC->MoveToActor(Player);
                        //AIC->MoveToSpecifiedLocation({0,0,0}, 1000.f);
                       
                        if (Count < 150)
                        {
                            EBTNodeResult::InProgress;
                        }
                        else
                        {
                            Count = 0;
                        }
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

    return EBTNodeResult::Succeeded;
}
