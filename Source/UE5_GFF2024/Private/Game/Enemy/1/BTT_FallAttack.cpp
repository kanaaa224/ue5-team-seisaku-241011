// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/1/BTT_FallAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Game/Enemy/1/Enemy1Character.h"
#include "Game//Enemy/1/AIC_Enemy1.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTT_FallAttack::UBTT_FallAttack()
{
	NodeName = "Custom Task";

    // カーブアセットの取得
    // カーブの種類によってクラスを選択(UCurveFloat / UCurveLinearColor / UCurveVector)
    const ConstructorHelpers::FObjectFinder<UCurveFloat>	_Find(TEXT("CurveFloat'/Game/Game/enemy/1/AI/CV_FallAttack.CV_FallAttack'"));
    if (_Find.Succeeded()) {
        MyCurve = _Find.Object;
    }

   
    

    IsTimelineStart = false;
    Velocity = { 0.,0.,0. };
    OldVelocity = { 0.,0.,10000. };
    Count = 0;
    StartLocationZ = -10000.f;
    IsStart = false;
}

EBTNodeResult::Type UBTT_FallAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (!MyTimelineComponent)
    {
        //MyTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent0"));
        MyTimelineComponent = NewObject<UTimelineComponent>(OwnerComp.GetOwner());
        MyTimelineComponent->RegisterComponent();
        //if (MyTimelineComponent) {
            // タイムライン更新時に呼び出されるメソッドの定義
            FOnTimelineFloat	_MyTimelineStepFunc;
            _MyTimelineStepFunc.BindUFunction(this, TEXT("TimelineStep"));
            MyTimelineComponent->AddInterpFloat(MyCurve, _MyTimelineStepFunc);

            // タイムライン終了時に呼び出されるメソッドの定義
            FOnTimelineEvent	_MyTimelineFinishedFunc;
            _MyTimelineFinishedFunc.BindUFunction(this, TEXT("TimelineFinished"));
            MyTimelineComponent->SetTimelineFinishedFunc(_MyTimelineFinishedFunc);
       // }
            IsStart = true;
    }
    


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
               
                    if (IsStart)
                    {
                        StartLocationZ = Enemy->GetActorLocation().Z;
                        IsStart = false;
                        Enemy->SetIsMoving(false);
                    }


                    FVector EnemyVector = Player->GetActorLocation() - Enemy->GetActorLocation();
                    FVector Normalize = EnemyVector / EnemyVector.Length();
                   
                 
                   

                    if (!IsTimelineStart)
                    {
                        IsTimelineStart = true;
                        MyTimelineComponent->PlayFromStart();
                    }

                   
                    if (Velocity.Z >= 0.9)
                    {
                        Velocity.X = Normalize.X * 50.;
                        Velocity.Y = Normalize.Y * 50.;
                    }
                    else
                    {
                        Velocity.X = 0.;
                        Velocity.Y = 0.;
                    }


                    //if (Velocity.Z >= 0.000001) {
                        FVector EnemyLocation;
                        EnemyLocation = Enemy->GetActorLocation() + Velocity;
                        EnemyLocation.Z = (Velocity.Z * 1000) + StartLocationZ;

                       
                        Enemy->TargetLocation = EnemyLocation;
                        Enemy->SetSpeed(10.f);
                        //UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(EnemyLocation.Z), true, true, FColor::Blue, 2.f);
                       
                    //}

                    if ((float)Velocity.Z <= 0.f)
                    {
                        Enemy->TargetLocation = Enemy->GetActorLocation();
                        Enemy->TargetLocation.Z = StartLocationZ;
                        Enemy->SetSpeed(10.f);
                    }

                    //落下中か
                    if (OldVelocity.Z > Velocity.Z)
                    {
                        Enemy->IsAttacking = true;
                    }
                    else
                    {
                        Enemy->IsAttacking = false;
                    }


                    if (Count < 3)
                    {
                        EBTNodeResult::InProgress;
                    }
                    else
                    {
                        Enemy->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
                        Enemy->SetIsMoving(true);
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

    Velocity = { 0,0,0 };
    Count = 0;
    return EBTNodeResult::Succeeded;
}

void UBTT_FallAttack::TimelineStep(float _Value)
{
    const double speed = 300.;

    OldVelocity.Z = Velocity.Z;
    Velocity.Z = _Value;
}

void UBTT_FallAttack::TimelineFinished()
{
    IsTimelineStart = false;

    Count++;
}
