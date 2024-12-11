// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"

//ロックオン
#include "../../System/LockOnInterface.h"

#include "Enemy2Character.generated.h"

//敵のHP
#define _ENEMY2_MAX_HP_ 100

//ダメージタイプと攻撃力
#define _DAMAGETYPE_NORMAL_ATTACK_ 0 //通常攻撃
#define _DAMAGE_NORMAL_ATTACK      10//通常攻撃のダメージ量

#define _DAMAGETYPE_ULT_ATTACK_    1 //特殊攻撃
#define _DAMAGE_ULT_ATTACK_        50//特殊攻撃のダメージ量

//レベル遷移する秒数
#define _SEC_CHANGE_LEVEL_          2.0f

//ダメージを受けた時にマテリアルを変える秒数
#define _SEC_CHANGE_DAMAGE_MATERIAL 0.3f

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

	float sec_ChangeDamegeMaterial = 0.0f;

	//スポーン時のLocationを取得
	FVector startLocation;

	//通常攻撃のエフェクト
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystemComponent* NormalSpark;

	//地面に落ちる時のエフェクト
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystemComponent* SpecialSpark;

public:
	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

	virtual void Destroyed() override;

public:
	void ApplyDamage(AActor* Other);

	//ダメージを受ける処理
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)override;

	/// <summary>
	/// 死亡処理
	/// </summary>
	void Die();

	//
	void DamageMaterial();
	void ChangeDamageMaterial();
	//
	void NormalMaterial();

	//特殊攻撃で自分がだすエフェクトをON
	void TrueSpecialSparkEffect();
	//特殊攻撃で自分がだすエフェクトをOFF
	void FalseSpecialSparkEffct();

	//通常攻撃でだすエフェクトをON
	void TrueNormalSparkEffect();
	//通常攻撃でだすエフェクトをOFF
	void FalseNormalSparkEffect();

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

	//特殊攻撃のアクタを生成する
	void SpawnAttackObject(int createNum);

	//スタート位置を取得
	FVector GetStartLocation();

private:
	//CubeMeshComponent
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CubeMesh;

public:
	bool No_ApplyDamage = false;
};
