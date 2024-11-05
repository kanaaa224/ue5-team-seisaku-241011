// Fill out your copyright notice in the Description page of Project Settings.
#include "Game/Enemy/2/Enemy2Character.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Game/Player_Cube.h"
#include "Game/Enemy/2/AIC_Enemy2.h"

#include "Math/UnrealMathUtility.h"

// Sets default values
AEnemy2Character::AEnemy2Character()
{
	/*:::::変数:::::*/
	//体力
	health = 100.0f;

	/*:::::関数:::::*/
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	// 視野
	PawnSensingComp->SetPeripheralVisionAngle(60.f);
	// 見える範囲
	PawnSensingComp->SightRadius = 2000;
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemy2Character::OnSeePlayer);

}

// Called when the game starts or when spawned
void AEnemy2Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy2Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy2Character::OnSeePlayer(APawn* Pawn)
{
	AAIC_Enemy2* AIController = Cast<AAIC_Enemy2>(GetController());
	// プレイヤー
	APlayer_Cube* player = Cast<APlayer_Cube>(Pawn);

	if (AIController && player)
	{
		// AIControllerにプレイヤー情報を設定
		AIController->SetPlayerKey(player);
	}

	// 視野に入ったら画面に"See"と表示
	UKismetSystemLibrary::PrintString(this, "See", true, true, FColor::Blue, 2.f);
}

bool AEnemy2Character::ApplyDamage(AActor* DamagedActor, float Damage, int DamageType)
{

	
	switch(DamageType){//敵がやった攻撃のダメージの種類別でダメージを与える
		case _DAMAGETYPE_NORMAL_ATTACK_://通常攻撃
			//DamagedActor->TakeDamage(_DAMAGE_NORMAL_ATTACK,,GetController(),this)
			break;

		case _DAMAGETYPE_ULT_ATTACK_://特殊攻撃

			break;
	}
	return false;
}

float AEnemy2Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//受けたダメージ量分HPを減産
	health -= DamageAmount;

	//ヘルスがゼロ以下なら死亡処理
	if (health <= 0) {
		//ここに死亡時に実行する処理、関数を書く
	}

	//受けたダメージ量を返す
	return DamageAmount;
}

