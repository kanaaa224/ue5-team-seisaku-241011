// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/2/BTT_NormalAttack_Enemy2.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Game/Enemy/2/AIC_Enemy2.h"

UBTT_NormalAttack_Enemy2::UBTT_NormalAttack_Enemy2(FObjectInitializer const& ObjectInitializer)
{
	lifting = false;
}

EBTNodeResult::Type UBTT_NormalAttack_Enemy2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//AIコントローラーを取得
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr){
		return EBTNodeResult::Failed;
	}

	//コントローラからPawnを取得
	APawn* ControlledPawn = AIController->GetPawn();
	if (ControlledPawn == nullptr) {
		return EBTNodeResult::Failed;
	}

	//:::::Rotation処理::::://
	//ActorのRotationを変数に代入
	FRotator CurrentRotaton = ControlledPawn->GetActorRotation();

	if (lifting == false) {
		CurrentRotaton.Pitch -= ATTACK_SPEED;
	}
	else if (lifting == true) {
		CurrentRotaton.Pitch += ATTACK_SPEED;
	}

	if (CurrentRotaton.Pitch <= -90.0f) {
		lifting = true;
	}
	else if (CurrentRotaton.Pitch >= 0.0f) {
		lifting = false;
	}
	
	//新しいRotationを設定
	ControlledPawn->SetActorRotation(CurrentRotaton);


	return EBTNodeResult::Succeeded;
}
