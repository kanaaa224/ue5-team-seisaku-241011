// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"

//ロックオン
#include "../../System/LockOnInterface.h"

#include "Enemy2Character.generated.h"

//ダメージタイプと攻撃力
#define _DAMAGETYPE_NORMAL_ATTACK_ 0 //通常攻撃
#define _DAMAGE_NORMAL_ATTACK      10//通常攻撃のダメージ量

#define _DAMAGETYPE_ULT_ATTACK_    1 //特殊攻撃
#define _DAMAGE_ULT_ATTACK_        50//特殊攻撃のダメージ量

UCLASS()
class UE5_GFF2024_API AEnemy2Character : public ACharacter , public ILockOnInterface
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
	void ApplyDamage(AActor* Other);

	//ダメージを受ける処理
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)override;

	/// <summary>
	/// 死亡処理
	/// </summary>
	void Die();

public:
	//ロックオン
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Widget, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* LockOnMarkerWidget;

	//ロックオンの有効フラグを設定する
	virtual void SetLockOnEnable_Implementation(bool LockOnFlg)override;

	//Get HP
	float GetHP();

	//BeginOverlap
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);

	//EndOverlap
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//プレイヤーに攻撃を与える
	void AttackPlayer();

private:
	//CubeMeshComponent
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CubeMesh;
};
