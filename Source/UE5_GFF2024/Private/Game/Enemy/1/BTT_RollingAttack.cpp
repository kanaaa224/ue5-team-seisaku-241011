// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/1/BTT_RollingAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Game/Enemy/1/Enemy1Character.h"
#include "Game//Enemy/1/AIC_Enemy1.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Game/Enemy/Commons/PolygonRotationManager.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

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
        //UKismetSystemLibrary::PrintString(this, "tes", true, true, FColor::Blue, 2.f);

        if (AAIC_Enemy1* AIC = Cast<AAIC_Enemy1>(OwnerComp.GetAIOwner()))
        {
            if (AEnemy1Character* Enemy = Cast<AEnemy1Character>(AIC->GetPawn()))
            {
                if (AActor* Player = Cast<AActor>(PlayerObject))
                {
                    FVector EnemyVector = Player->GetActorLocation() - Enemy->GetActorLocation();


                    if (++Count < 60)
                    {
                        Enemy->SetIsMoving(false);
                        FRotator LookAtRotation = FRotationMatrix::MakeFromX(Player->GetActorLocation() - Enemy->GetActorLocation()).Rotator();
                        //Enemy->SetActorRotation({ 0,LookAtRotation.Yaw,0 });
                        Vector = Player->GetActorLocation() - Enemy->GetActorLocation();


                        FVector TmpVector = { Player->GetActorLocation().X - Enemy->GetActorLocation().X
                              ,Player->GetActorLocation().Y - Enemy->GetActorLocation().Y,Enemy->GetActorLocation().Z };

                        Target = TmpVector / TmpVector.Length();
                        Target.Z = Enemy->GetActorLocation().Z;


                        Enemy->TargetLocation.X = -1 * Target.X * 50 + Enemy->GetActorLocation().X;
                        Enemy->TargetLocation.Y = -1 * Target.Y * 50 + Enemy->GetActorLocation().Y;
                        Enemy->TargetLocation.Z = Target.Z;
                        Enemy->SetSpeed(5.f);


                        return EBTNodeResult::Succeeded;
                    }
                    else
                    {
                        FVector Normalize = Vector / Vector.Length();
                        Normalize = { Normalize.X, Normalize.Y, 0. };
                        Enemy->SetIsMoving(false);
                        //Enemy->SetActorLocation(Enemy->GetActorLocation() + Normalize * 10);
                        //Enemy->AddMovementInput(Normalize, 100.f);
                        //AIC->MoveToSpecifiedLocation(Enemy->GetActorLocation() + Normalize * 3, 10.f);
                        //AIC->MoveToActor(Player);
                      
                        Enemy->IsAttacking = true;

                       /* if ((int)Target.X == 0)
                        {
                            Enemy->TargetLocation.X = Target.X * 40 + Player->GetActorLocation().X;
                            Enemy->TargetLocation.Y = Target.Y * 40 + Player->GetActorLocation().Y;
                            Enemy->TargetLocation.Z = Target.Z;
                            Enemy->SetSpeed(5.f);
                        }*/

                        FVector tmp = { };


                        if (!IsAttacked)
                        {
                            Enemy->TargetLocation.X = Target.X * 3000 + Enemy->GetActorLocation().X;
                            Enemy->TargetLocation.Y = Target.Y * 3000 + Enemy->GetActorLocation().Y;
                            Enemy->TargetLocation.Z = Target.Z;
                        

                            IsAttacked = true;

                            TargetLocation = Enemy->TargetLocation;
                        }

                        if (Speed += 0.5f)
                        {
                            Enemy->SetSpeed(Speed);
                        }
                       
                       /* Enemy->TargetLocation.X = Target.X * 100 + Enemy->GetActorLocation().X;
                        Enemy->TargetLocation.Y = Target.Y * 100 + Enemy->GetActorLocation().Y;
                        Enemy->TargetLocation.Z = Target.Z;*/

                        Enemy->MoveDirection = { Target.X * 100 ,Target.Y * 100 ,Target.Z };
                        Enemy->AttackState = 2;
                        
                        

                       //FVector tmp0 = { Enemy->TargetLocation.X ,Enemy->TargetLocation.Y ,Target.Z };
                       // FVector tmp1 = Enemy->GetActorLocation() - TargetLocation;

                        // エフェクトをスポーン
                        if (NiagaraComp == nullptr)
                        {
                            FVector SpawnLocation = Enemy->GetActorLocation() + LocationOffset;
                            NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                                Enemy->GetWorld(),
                                NiagaraEffect,
                                SpawnLocation,
                                FRotator::ZeroRotator,
                                FVector(4.0f)
                            );

                            IsSpawnNiagara = false;

                            //FTimerHandle TimerHandle;
                            //GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, NiagaraComp]()
                            //    {
                            //        NiagaraComp->DestroyComponent();
                            //    }, 0.1f, false);  // 2秒後に無効化
                        }
                        else
                        {
                            NiagaraComp->SetWorldLocation(Enemy->GetActorLocation());
                        }

                       
                        if (/*tmp1.Length() > 100*/Count < 120)
                        {
                            //return EBTNodeResult::InProgress;
                            IsSpawnNiagara = true;
                            return EBTNodeResult::Succeeded;

                        }
                        else
                        {
                            Count = 0;
                            Enemy->GetCharacterMovement()->MaxWalkSpeed = 600.0f;
                            //Enemy->SetIsMoving(true);
                            NiagaraComp->DestroyComponent();


                            AIC->SetState(3);
                            AIC->SetNextState(0);
                            Enemy->TargetLocation = { -1, -1, -10000 };
                            Enemy->GetPolygonRotationManager()->Init();
                            //Enemy->SetActorRotation({ 0,0,0 });
                            Speed = 0;
                            IsAttacked = false;
                        }
                    }
                }
            }
        }

    }
    else
    {
        // `Target` が設定されていない場合の処理
        return EBTNodeResult::Failed;
    }

    // タスク実行後、`Target` に新しい値を設定する例
   // BlackboardComp->SetValueAsObject(GetSelectedBlackboardKey(), nullptr); // 例として `null` に設定
    Count = 0;
    return EBTNodeResult::Succeeded;
}
