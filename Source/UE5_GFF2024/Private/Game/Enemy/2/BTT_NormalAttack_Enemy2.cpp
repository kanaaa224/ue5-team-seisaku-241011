// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/2/BTT_NormalAttack_Enemy2.h"
#include "Kismet/KismetSystemLibrary.h"

//Enemy2のAIコントローラ
#include "Game/Enemy/2/AIC_Enemy2.h"
//Enemy2のキャラクター
#include "Game/Enemy/2/Enemy2Character.h"

#include "Sound/SoundBase.h"
#include "GameFramework/Actor.h"
#include <Kismet/GameplayStatics.h>
#include <Perception/AIPerceptionComponent.h>

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

	// 効果音を動的にロード
	static ConstructorHelpers::FObjectFinder<USoundBase> SoundEffectObj(TEXT("/Game/Game/enemy/2/SE/NormalAttack.NormalAttack"));
	if (SoundEffectObj.Succeeded())
	{
		SoundEffect = SoundEffectObj.Object;
	}
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
	AEnemy2Character* MyPawn = Cast<AEnemy2Character>(ControlledPawn);
	if (MyPawn == nullptr) {
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
		//Enemy2Characterクラスの変数をTrue（プレイヤーにダメージを与えない）
		MyPawn->No_ApplyDamage = true;

		frameCnt_Jump++;
		nowLocation.operator+=(FVector(0.0f, 0.0f, 50.0f));
		nowRotaton.operator+=(FRotator(1.5f,0.0,0.0));

		if (frameCnt_Jump == 10) {
			endJump = true;
		}
	}

	//攻撃振り下ろす
	if (endJump == true && endAttack == false) {
		//Enemy2Characterクラスの変数をFalse（プレイヤーにダメージを与える）
		MyPawn->No_ApplyDamage = false;

		startAttack = true;
		frameCnt_Attack_Down++;

		nowLocation.operator-=(FVector(0.0f, 0.0f, 70.0f));
		nowRotaton.operator-=(FRotator(17.0f,0.0f,0.0f));
		//FMath::RInterpTo(nowRotaton,FRotator(90.0f,0.0f,0.0f), )

		if (frameCnt_Attack_Down == 10) {
			endAttack = true;
			//MyPawn->TrueNormalSparkEffect();
			UE_LOG(LogTemp, Log, TEXT("SparkEffect------------>true"));
			//効果音の再生
			PlaySoundEffect(MyPawn);
		}
	}

	//攻撃から起き上がる
	if (endJump == true && endAttack == true && endTask == false) {
		frameCnt_Attack_Up++;
		//MyPawn->FalseNormalSparkEffect();
		UE_LOG(LogTemp, Log, TEXT("SparkEffect------------>false"));

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

void UBTT_NormalAttack_Enemy2::PlaySoundEffect(AEnemy2Character* pawn)
{
	if (SoundEffect)
	{
		// 効果音を再生
		UGameplayStatics::PlaySoundAtLocation(this, SoundEffect, pawn->GetActorLocation());
	}
}
