// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/2/BTT_Move_Enemy2.h"

//Enemy2Controller
#include "Game/Enemy/2/AIC_Enemy2.h"
//PlayController
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

UBTT_Move_Enemy2::UBTT_Move_Enemy2(FObjectInitializer const& ObjectInitializer)
{
	StopMove = false;
}

EBTNodeResult::Type UBTT_Move_Enemy2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//AIコントローラーを取得
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr) {
		return EBTNodeResult::Failed;
	}
	//コントローラからPawnを取得
	APawn* ControlledPawn = AIController->GetPawn();
	if (ControlledPawn == nullptr) {
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

	//現在のRotationを変数に代入
	NowRotation = ControlledPawn->GetActorRotation();

	//敵が傾いていたら元に戻す。プレイヤーまでの移動はストップ
	if (NowRotation.Pitch != 0.0f) {
		StopMove = true;
		NowRotation.Pitch += 3;

		if (NowRotation.Pitch >= 0.0f) {
			NowRotation.Pitch = 0.0f;
		}

		ControlledPawn->SetActorRotation(NowRotation);
	}
	else if (NowRotation.Pitch == 0.0f) {
		StopMove = false; 
		UKismetSystemLibrary::PrintString(GetWorld(), "StopMove : false");
	}

	//プレイヤーまでの移動処理
	if (StopMove == false) {
		//プレイヤーの方向を見る//
		Direction = PlayerPawn->GetActorLocation() - ControlledPawn->GetActorLocation();
		Direction.Z = 0.0f;

		TargetRotation = Direction.Rotation();

		ControlledPawn->SetActorRotation(TargetRotation);

		//プレイヤーまで移動//
		Direction = PlayerPawn->GetActorLocation() - ControlledPawn->GetActorLocation();
		Direction.Normalize();

		NewLocation = ControlledPawn->GetActorLocation() + Direction * 3;
		ControlledPawn->SetActorLocation(NewLocation);
	}

	return EBTNodeResult::Succeeded;
}
