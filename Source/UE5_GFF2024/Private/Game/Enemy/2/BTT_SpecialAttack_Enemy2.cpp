// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/2/BTT_SpecialAttack_Enemy2.h"

//Enemy2Controller
#include "Game/Enemy/2/AIC_Enemy2.h"
//PlayController
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
//Enemy2AttackObject
#include "Game/Enemy/2/Enemy2AttackObject.h"

UBTT_SpecialAttack_Enemy2::UBTT_SpecialAttack_Enemy2(FObjectInitializer const& ObjectInitializer)
{
	stopMove = false;

	SpecialAttackFlg = "SpecialAttackFlg";
	CoolTime = "CoolTime";
}

void UBTT_SpecialAttack_Enemy2::Init()
{
	stopMove = false;
	onecCalcFTL_Flg = false;
	floatEnd = false;
	downEnd = false;
	endCreateObject = false;
	onecCalcDTL_Flg = false;
}

EBTNodeResult::Type UBTT_SpecialAttack_Enemy2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Log, TEXT("ExecuteTask : SpecialAttack"));

	if (initFlg == false) {
		Init();
		initFlg = true;
	}

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

	//通常攻撃中だった場合はすぐに攻撃にうつらず体制を戻す
	FRotator checkRotator = MyPawn->GetActorRotation();
	if (checkRotator.Pitch != 0.0f) {
		stopMove = true;
		checkRotator.Pitch += 3.0f;

		if (checkRotator.Pitch >= 0.0f) {
			checkRotator.Pitch = 0.0f;
			stopMove = false;
		}

		MyPawn->SetActorRotation(checkRotator);
	}
	else if (checkRotator.Pitch == 0.0f) {
		stopMove = false;
	}

	//BlackboardのComponentを変数に代入
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();


	//
	//ブラックボードのNormalAttack変数を立てる
	ensure(BlackboardComp);
	BlackboardComp->SetValueAsBool(SpecialAttackFlg, true);

	//浮き上がり処理
	if (stopMove == false) {
		Float(MyPawn, OnecCalcFloatTargetLocation(MyPawn));
	}
	//棒の生成処理
	if (floatEnd == true) {
		//UE_LOG(LogTemp, Log, TEXT("Float End -----> next CreateActor"));
		if (endCreateObject == false) {
			for (int i = 1; i <= 4; i++) {
				//Objectを１つ生成する
				MyPawn->SpawnAttackObject(i);
				if (i == 4) {
					endCreateObject = true;
				}
			}
		}
	}
	//地面に降りる処理
	ULevel* CurrentLevel = GetWorld()->GetCurrentLevel();
	if (AttackObjOfLevel(CurrentLevel, AEnemy2AttackObject::StaticClass()) == false && endCreateObject == true) {
		//UE_LOG(LogTemp, Warning, TEXT("false -----------> DownObject"));
		Down(MyPawn, OnecCalcDownTargetLocation(MyPawn));
	}
	if (downEnd == true) {
		//ブラックボード変数のSpecialAttackFlgをfalseにする
		ensure(BlackboardComp);
		BlackboardComp->SetValueAsBool(SpecialAttackFlg, false);
		ensure(BlackboardComp);
		BlackboardComp->SetValueAsBool(CoolTime, true);
		initFlg = false;
	}

	return EBTNodeResult::Succeeded;
}

void UBTT_SpecialAttack_Enemy2::Float(AEnemy2Character* myPawn, FVector targetLocation)
{
	FVector nowLocation = myPawn->GetActorLocation();

	nowLocation = FMath::VInterpTo(nowLocation, targetLocation, GetWorld()->GetDeltaSeconds(), 3.0f);

	myPawn->SetActorLocation(nowLocation);
}

FVector UBTT_SpecialAttack_Enemy2::OnecCalcFloatTargetLocation(AEnemy2Character* pawn)
{
	FVector tmp = pawn->GetActorLocation();
	
	if (onecCalcFTL_Flg == false) {
		calcResultFTL = FVector(tmp.X, tmp.Y, tmp.Z + 1600.0f);//数字の部分で上昇する値を変える
		onecCalcFTL_Flg = true;
	}
	else if (onecCalcFTL_Flg == true) {
		if (calcResultFTL.Z - 30.0f <= tmp.Z) {
			floatEnd = true;
		}
	}

	return calcResultFTL;
}

void UBTT_SpecialAttack_Enemy2::Down(AEnemy2Character* myPawn, FVector targetLocation)
{
	//debugLog
	//UE_LOG(LogTemp, Warning, TEXT("Down"));

	FVector nowLocation = myPawn->GetActorLocation();

	//１次元の線形補間（Z軸）
	nowLocation.Z = nowLocation.Z + (GetWorld()->GetDeltaSeconds() * 3.0f) * ((targetLocation.Z - 1500.0f) - nowLocation.Z);

	//計算時にでた差異を修正
	if (nowLocation.Z <= 351.0f) {
		nowLocation.Z = 330.0f;
		downEnd = true;
	}

	//debugLog
	//UE_LOG(LogTemp, Warning, TEXT("nowLocation : X:%f Y:%f Z:%f"), nowLocation.X, nowLocation.Y, nowLocation.Z);
	myPawn->SetActorLocation(nowLocation);
}

FVector UBTT_SpecialAttack_Enemy2::OnecCalcDownTargetLocation(AEnemy2Character* pawn)
{
	FVector tmp = pawn->GetActorLocation();

	if (onecCalcDTL_Flg == false) {
		calcResultDTL = FVector(tmp.X, tmp.Y, tmp.Z + (-1600.0f));//数字の部分は上昇する値と同じ
		onecCalcDTL_Flg = true;
	}
	//else if (onecCalcDTL_Flg == true) {
	//	if (calcResultDTL.Z <= tmp.Z) {
	//		downEnd = true;
	//	}
	//}

	return calcResultDTL;
}

bool UBTT_SpecialAttack_Enemy2::AttackObjOfLevel(ULevel* Level, TSubclassOf<AActor> ActorClass)
{
	if (!Level || !ActorClass)
	{
		return false;
	}

	// Level内のアクターを検索
	for (AActor* Actor : Level->Actors)
	{
		if (Actor && Actor->IsA(ActorClass))
		{
			return true;
		}
	}

	return false;
}
