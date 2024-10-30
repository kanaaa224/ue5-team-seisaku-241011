// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy1Character.generated.h"

UCLASS()
class UE5_GFF2024_API AEnemy1Character : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy1Character();

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
	//ダメージを受ける処理
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)override;

public:
	//float GetDistanceFromPlayer(APlayer_Cube* player);
};
