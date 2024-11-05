// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/2/BTS_SetParameter.h"
//PlayController
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
//AIController
#include "Game/Enemy/2/AIC_Enemy2.h"
//Blackborad
#include "BehaviorTree/BlackboardComponent.h"
//Debug用にPrintStringを使用するためにインクルード
#include "Kismet/KismetSystemLibrary.h"

UBTS_SetParameter::UBTS_SetParameter()
{
	//ブラックボードにある変数を設定
	DistanceKeyName = "Distance";
}

void UBTS_SetParameter::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//BlackboardのComponentを変数に代入
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	//プレイヤーのLocationを取得
	//プレイヤーコントローラーを取得
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//コントローラからPawnを取得
	APawn* PlayerPawn = PlayerController->GetPawn();
	FVector pl = PlayerPawn->GetActorLocation();

	//自分のLocationを取得
	//AIコントローラーを取得
	AAIController* AIController = OwnerComp.GetAIOwner();
	//コントローラからPawnを取得
	APawn* ControlledPawn = AIController->GetPawn();
	FVector ml = ControlledPawn->GetActorLocation();

	//自分の現在地からプレイヤーまでの距離を計算し変数に代入
	float distance = FVector::Dist(ml, pl);

	//ブラックボードにあるfloat型のDistance変数に代入
	ensure(BlackboardComp);
	BlackboardComp->SetValueAsFloat(DistanceKeyName, distance);

	//UKismetSystemLibrary::PrintString(GetWorld(), FString::SanitizeFloat(distance));
}
