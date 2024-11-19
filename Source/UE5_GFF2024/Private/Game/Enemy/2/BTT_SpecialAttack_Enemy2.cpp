// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/2/BTT_SpecialAttack_Enemy2.h"

//Enemy2Controller
#include "Game/Enemy/2/AIC_Enemy2.h"
//Enemy2のキャラクター
#include "Game/Enemy/2/Enemy2Character.h"
//PlayController
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

UBTT_SpecialAttack_Enemy2::UBTT_SpecialAttack_Enemy2(FObjectInitializer const& ObjectInitializer)
{
	stopMove = false;
}

void UBTT_SpecialAttack_Enemy2::Init()
{
}

EBTNodeResult::Type UBTT_SpecialAttack_Enemy2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	MyPawn->ULT_Float();

	UE_LOG(LogTemp, Log, TEXT("SpecialAttack"));
	return EBTNodeResult::Succeeded;
}
