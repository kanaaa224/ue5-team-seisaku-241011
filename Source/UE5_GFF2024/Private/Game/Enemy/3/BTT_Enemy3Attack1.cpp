// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/3/BTT_Enemy3Attack1.h"
#include "Game/Enemy/3/Enemy3Character.h"
#include "Game/Enemy/3/AIC_Enemy3.h"

#include "Kismet/GameplayStatics.h"

UBTT_Enemy3Attack1::UBTT_Enemy3Attack1()
{

}

//タスクが動く この中に動かしたいことを書く
EBTNodeResult::Type UBTT_Enemy3Attack1::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	//AIコントローラーを取得
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr) {
		return EBTNodeResult::Failed;
	}
	//コントローた(AIC_Enemy2)を取得
	AAIC_Enemy3* MyAIController = Cast<AAIC_Enemy3>(AIController);
	if (MyAIController == nullptr) {
		return EBTNodeResult::Failed;
	}
	//コントローラ(AIC_Enemy2)からPawnを取得
	APawn* ControlledPawn = MyAIController->GetPawn();
	if (ControlledPawn == nullptr) {
		return EBTNodeResult::Failed;
	}
	//Enemy3のTransformなどがMyPawnから取得できる
	//Pawnの中には、Enemy3のTransformが入っているらしい
	AEnemy3Character* MyPawn = Cast<AEnemy3Character>(ControlledPawn);
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

	MyPawn->BTT_TestLog();
	

	//サクセスじゃないとError
	return EBTNodeResult::Succeeded;
}
