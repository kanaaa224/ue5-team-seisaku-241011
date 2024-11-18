// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/2/BTT_NormalAttack_Enemy2.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Game/Enemy/2/AIC_Enemy2.h"

UBTT_NormalAttack_Enemy2::UBTT_NormalAttack_Enemy2(FObjectInitializer const& ObjectInitializer)
{
	//ブラックボードにある変数を設定
	AttackKeyName = "Attack";
	ExeNormalAttackKeyName = "ExecutionNormalAttack";

	initFlg = false;
	endAttack = false;
	startAttack = true;
	endJump = false;
	frameCnt_Jump = 0;
	startStandUp = false;
}

void UBTT_NormalAttack_Enemy2::Init()
{
	endAttack = false;
	startAttack = false;
	frameCnt_Attack_Down = 0;

	frameCnt_Attack_Up = 0;

	endJump = false;
	frameCnt_Jump = 0;
	startStandUp = false;

	endTask = false;
}

EBTNodeResult::Type UBTT_NormalAttack_Enemy2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//初期化
	if (initFlg == false) {
		Init();
		initFlg = true;
	}

	//BlackboardのComponentを変数に代入
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) {
		return EBTNodeResult::Failed;
	}

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

	//ブラックボードのNormalAttack変数を立てる
	ensure(BlackboardComp);
	BlackboardComp->SetValueAsBool(ExeNormalAttackKeyName, true);

	//:::::攻撃アニメーション処理::::://
	//現在のRotateを取得
	nowRotaton = ControlledPawn->GetActorRotation();
	//現在のLocationを取得
	nowLocation = ControlledPawn->GetActorLocation();

	//攻撃前にジャンプ処理
	if (endJump == false) {
		frameCnt_Jump++;
		nowLocation.operator+=(FVector(0.0f, 0.0f, 50.0f));
		nowRotaton.operator+=(FRotator(1.5f,0.0,0.0));

		if (frameCnt_Jump == 10) {
			endJump = true;
		}
	}

	//攻撃振り下ろす
	if (endJump == true && endAttack == false) {
		startAttack = true;
		frameCnt_Attack_Down++;

		nowLocation.operator-=(FVector(0.0f, 0.0f, 70.0f));
		nowRotaton.operator-=(FRotator(17.0f,0.0f,0.0f));

		if (frameCnt_Attack_Down == 10) {
			endAttack = true;
		}
	}

	//攻撃から起き上がる
	if (endJump == true && endAttack == true && endTask == false) {
		frameCnt_Attack_Up++;

		nowLocation.operator+=(FVector(0.0f, 0.0f, 20.0f));

		if (frameCnt_Attack_Up == 10) {
			endTask = true;
		}
	}

	//新しいRotationを設定
	//UE_LOG(LogTemp, Log, TEXT("Pitch : %f"), nowRotaton.Pitch);
	ControlledPawn->SetActorRelativeRotation(nowRotaton);
	//新しいLocationを設定
	ControlledPawn->SetActorRelativeLocation(nowLocation);

	if (endTask == true) {
		//攻撃Flgをfalseに戻す
		ensure(BlackboardComp);
		BlackboardComp->SetValueAsBool(AttackKeyName, false);
		ensure(BlackboardComp);
		BlackboardComp->SetValueAsBool(ExeNormalAttackKeyName, false);
		UE_LOG(LogTemp, Log, TEXT("SetAttack : false"));

		initFlg = false;
	}
	return EBTNodeResult::Succeeded;
}

FRotator UBTT_NormalAttack_Enemy2::CombineRotators(FRotator R1, FRotator R2)
{
	FRotator result = R1 + R2;

	return FRotator(result);
}
