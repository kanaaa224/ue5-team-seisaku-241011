// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy2Character.generated.h"

//ダメージタイプと攻撃力
#define _DAMAGETYPE_NORMAL_ATTACK_ 0 //通常攻撃
#define _DAMAGE_NORMAL_ATTACK      10//通常攻撃のダメージ量

#define _DAMAGETYPE_ULT_ATTACK_    1 //特殊攻撃
#define _DAMAGE_ULT_ATTACK_        50//特殊攻撃のダメージ量

UCLASS()
class UE5_GFF2024_API AEnemy2Character : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy2Character();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	/*:::::変数:::::*/
	//体力
	float health;

public:
	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

public:
	/// <summary>
	/// ダメージを与える
	/// </summary>
	/// <param name="DamagedActor">ダメージを与えるActor</param>
	/// <param name="Damage">与えるダメージ</param>
	/// <param name="DamageType">与えるダメージの種類（ヘッダーで宣言したマクロを使う）</param>
	/// <returns>ダメージを与えられたか</returns>
	bool ApplyDamage(AActor* DamagedActor, float Damage, int DamageType);

	//ダメージを受ける処理
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)override;
};
