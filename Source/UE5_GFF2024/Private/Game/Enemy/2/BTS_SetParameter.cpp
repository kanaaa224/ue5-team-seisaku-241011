// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/2/BTS_SetParameter.h"
//PlayController
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
//AIController
#include "Game/Enemy/2/AIC_Enemy2.h"
//Character
#include "Game/Enemy/2/Enemy2Character.h"
//Blackborad
#include "BehaviorTree/BlackboardComponent.h"
//Debug用にPrintStringを使用するためにインクルード
#include "Kismet/KismetSystemLibrary.h"

UBTS_SetParameter::UBTS_SetParameter()
{
	//ブラックボードにある変数を設定
	DistanceKeyName = "Distance";
	AttackKeyName = "Attack";
	HPKeyName = "MyHealth";
	HPRatioKeyName = "HPRatio";
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

	//プレイヤーまでの距離に応じて攻撃FlgをONにする
	if (distance <= 450 && BlackboardComp->GetValueAsBool(AttackKeyName) != true) {
		ensure(BlackboardComp);
		BlackboardComp->SetValueAsBool(AttackKeyName, true);
		UE_LOG(LogTemp, Log, TEXT("SetAttack : true"));
	}

	float HP;
	// プレイヤーキャラクターのインスタンスを取得
	AEnemy2Character* EnemyCharacter = Cast<AEnemy2Character>(ControlledPawn);
	if (EnemyCharacter){
		HP = EnemyCharacter->GetHP();
		UE_LOG(LogTemp, Log, TEXT("Enemy2 HP : %f"), HP);
	}
	//ブラックボードにあるfloat型のMyHealth変数に代入
	ensure(BlackboardComp);
	BlackboardComp->SetValueAsFloat(HPKeyName, HP);
	
	float HPRatio = HP / _ENEMY2_MAX_HP_;
	//ブラックボードにあるfloat型のHPRatio変数に代入
	ensure(BlackboardComp);
	BlackboardComp->SetValueAsFloat(HPRatioKeyName, HPRatio);
}
