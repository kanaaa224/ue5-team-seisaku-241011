// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
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
	int Beam_Effect_Collision();

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

private:

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
