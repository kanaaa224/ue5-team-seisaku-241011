// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy2AttackObject.generated.h"

#define _ATTACK_DAMAGE_ 50.0f //攻撃力
#define _ATTACK_SPEED_  30.0f //攻撃時にプレイヤー目掛けて飛ぶ時の速度

/*
timeToAttackが０になったら
↓
プレイヤーの位置情報を取得
↓
その位置まで飛ぶ 
*/

UCLASS()
class UE5_GFF2024_API AEnemy2AttackObject : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy2AttackObject();
	AEnemy2AttackObject(float timeToAttack);

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
	//生成した時のワールド位置
	FVector createPosition;

	//攻撃までの時間
	float timeToAttack;

	//自分が何番目に生成されたか
	int createNumber;

	//生成されて何秒たったか
	float secTime = 0.0f;

	//＊＊＊＊＊メンバ関数＊＊＊＊＊//
public:
	void SetCreateNumber(int createNum);
};

