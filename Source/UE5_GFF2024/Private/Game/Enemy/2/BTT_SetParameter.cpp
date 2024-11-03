// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/2/BTT_SetParameter.h"

//PlayController
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
//AIController
#include "Game/Enemy/2/AIC_Enemy2.h"
//Blackborad
#include "BehaviorTree/BlackboardComponent.h"
//Debug用にPrintStringを使用するためにインクルード
#include "Kismet/KismetSystemLibrary.h"

UBTT_SetParameter::UBTT_SetParameter(FObjectInitializer const& ObjectInitializer)
{
	//BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	//(TEXT("/Game/Game/enemy/2/AI/BT_Enemy2.BT_Enemy2"));

	//ブラックボードにある変数を設定
	DistanceKeyName = "Distance";
}

EBTNodeResult::Type UBTT_SetParameter::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//BlackboardのComponentを変数に代入
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) {
		return EBTNodeResult::Failed;
	}

	//プレイヤーのLocationを取得
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
	FVector pl = PlayerPawn->GetActorLocation();

	//自分のLocationを取得
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
	FVector ml = ControlledPawn->GetActorLocation();

	//自分の現在地からプレイヤーまでの距離を計算し変数に代入
	float distance = FVector::Dist(ml, pl);

	//ブラックボードにあるfloat型のDistance変数に代入
	ensure(BlackboardComp);
	BlackboardComp->SetValueAsFloat(DistanceKeyName, distance);

	UKismetSystemLibrary::PrintString(GetWorld(), FString::SanitizeFloat(distance));

	//ビヘイビアーに成功を返す
	return EBTNodeResult::Succeeded;
}
