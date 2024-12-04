// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "Game/Enemy/Commons/PolygonRotationManager.h"

#include "Enemy3Character.generated.h"

UCLASS()
class UE5_GFF2024_API AEnemy3Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy3Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

	UFUNCTION()
	void Attack_Beam_Up();

	UFUNCTION()
	void Attack_Beam_Effect();

	UFUNCTION()
	void Beam_Collision_Spawn();

private:

	/* DiceMehComponent */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> DiceMesh;

	/* 当たり判定用 BoxComponent */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> HitBox;

	/** Niagaraシステムの参照 */
	UNiagaraSystem* NiagaraEffect;

	/* 攻撃中にEffectを表示させる用フラグ */
	bool EffectSpawnFlg = false;

	/* */
	PolygonRotationManager* polygonrotation;

	/* 立方体の頂点を定義 */
	TArray<FVector> CubeVertices = {
		FVector(-1, -1, -1), FVector(1, -1, -1), FVector(1, 1, -1), FVector(-1, 1, -1),  // 下面
		FVector(-1, -1,  1), FVector(1, -1,  1), FVector(1,  1,  1), FVector(-1,  1,  1)   // 上面
	};

	// 立方体の6つの面を定義（各面は頂点インデックスのリスト）
	TArray<TArray<int32>> CubeFaces = {
		{0, 1, 2, 3},  // 底面
		{4, 5, 6, 7},  // 上面
		{0, 1, 5, 4},  // 前面
		{2, 3, 7, 6},  // 後面
		{0, 3, 7, 4},  // 左面
		{1, 2, 6, 5}   // 右面
	};

public:

	/* 移動方向 */
	FVector movement = FVector(1.0, 1.0, 0.0);

private:

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/* 移動する時の回転や方向などを決める */
	UFUNCTION()
	void MoveProcess();
};
