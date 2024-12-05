// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

//Player
#include "Game/Player_Cube.h"
//Enemy2のAIコントローラ
#include "Game/Enemy/2/AIC_Enemy2.h"
//Enemy2のキャラクター
#include "Game/Enemy/2/Enemy2Character.h"
//Particle
#include "Particles/ParticleSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "Enemy2AttackObject.generated.h"

#define _ATTACK_DAMAGE_ 50.0f //攻撃力
#define _ATTACK_SPEED_  30.0f //攻撃時にプレイヤー目掛けて飛ぶ時の速度

UCLASS()
class UE5_GFF2024_API AEnemy2AttackObject : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy2AttackObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//＊＊＊＊＊メンバ変数＊＊＊＊＊//
private:
	//CubeMeshComponent
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CubeMesh;

public:
	//Playerクラス
	AActor* player;

	//初期化したか
	bool InitFlg = false;

	//生成した時のワールド位置
	FVector createPosition;

	//攻撃までの時間
	float timeToAttack;
	//攻撃を始める
	bool beginAttackFlg = false;

	//自分が何番目に生成されたか
	int createNumber;

	//生成されて何秒たったか
	float secTime = 0.0f;
	//攻撃開始から何秒たったか
	float secAttackTime = 0.0f;

	//移動をやめる
	bool stopMove = false;

	//このクラスを破棄するまでの時間
	float secDestoryTime = 0.0f;

	//攻撃を終えたか
	bool endAttack = false;

	

private:
	//Niageraエフェクトのアセット
	UPROPERTY(EditAnywhere,Category = "Effects")
	UNiagaraComponent* Fire;

	//地面についた時のエフェクト
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystemComponent* Bomb;
	//Bombエフェクトを一回のみ
	bool bombFlg = false;

	//＊＊＊＊＊メンバ関数＊＊＊＊＊//
public:
	//生成時の番号を設定
	void SetCreateNumber(int createNum);
	//生成時の番号を取得
	int GetCreateNumber();

	//プレイヤーの方向を向く
	void RotateTowardsTarget(float deltaTime);

	//プレイヤーに頭を向ける
	void HeadToTarget(float deltaTime);

	//プレイヤーまで移動
	void TargetToMove(float deltaTime);

	//BeginOverlap
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, 
						AActor* OtherActor, 
						UPrimitiveComponent* OtherComp, 
						int32 OtherBodyIndex, 
						bool bFromSweep, 
						const FHitResult& SweepResult);

private:
	//エフェクトをスポーンする関数
	void SpawnFireEffect();
	//爆発エフェクトをスポーンする関数
	void SpawnBombEffect();
};

