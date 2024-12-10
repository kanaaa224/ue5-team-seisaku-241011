// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/2/BTT_NormalAttack_2_Enemy2.h"
//Enemy2Controller
#include "Game/Enemy/2/AIC_Enemy2.h"
//PlayController
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "Kismet/KismetSystemLibrary.h"

UBTT_NormalAttack_2_Enemy2::UBTT_NormalAttack_2_Enemy2(FObjectInitializer const& ObjectInitializer)
{
	
}

EBTNodeResult::Type UBTT_NormalAttack_2_Enemy2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//AIコントローラーを取得
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr) {
		return EBTNodeResult::Failed;
	}
	//コントローた(AIC_Enemy2)を取得
	AAIC_Enemy2* MyAIController = Cast<AAIC_Enemy2>(AIController);
	if (MyAIController == nullptr) {
		return EBTNodeResult::Failed;
	}
	//コントローラ(AIC_Enemy2)からPawnを取得
	APawn* ControlledPawn = MyAIController->GetPawn();
	if (ControlledPawn == nullptr) {
		return EBTNodeResult::Failed;
	}
	AEnemy2Character* MyPawn = Cast<AEnemy2Character>(ControlledPawn);
	if (MyPawn == nullptr) {
		return EBTNodeResult::Failed;
	}

	//プレイヤーコントローラーを取得
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController == nullptr) {
		return EBTNodeResult::Failed;
	}
	//コントローラからPawnを取得
	APawn* PlayerPawn = PlayerController->GetPawn();
	if (PlayerPawn == nullptr) {
		return EBTNodeResult::Failed;
	}

	//BlackboardのComponentを変数に代入
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	//--------------------------------------------------------------//
	//倒れる処理
	Fall(MyPawn);


	return EBTNodeResult::Succeeded;
}

void UBTT_NormalAttack_2_Enemy2::Fall(AEnemy2Character* pawn)
{
	if (doOnec_Get_TargetRotation == false) {
		targetRotation = pawn->GetActorRotation();
		targetRotation.Pitch = 90.0f;
		doOnec_Get_TargetRotation = true;
	}
	FRotator nowRotation = pawn->GetActorRotation();
	FRotator resultRotation = targetRotation;
	
	UE_LOG(LogTemp, Log, TEXT("Pith %f"), pawn->GetActorRotation().Pitch);

	pawn->SetActorRelativeRotation(resultRotation);
}