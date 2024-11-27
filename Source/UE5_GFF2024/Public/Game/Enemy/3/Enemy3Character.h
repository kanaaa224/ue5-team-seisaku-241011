// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
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
	void BTT_EnemyLog();

	UFUNCTION()
	bool BTT_Enemy3Attack_Beam(AEnemy3Character* _mypawn);

	UFUNCTION()
	void BTT_Enemy3Attack_Beam2();

private:

	/* DiceMehComponent */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> DiceMesh;

	/* 当たり判定用 BoxComponent */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> HitBox;

private:

	//FVector型は、TransformにあたるLocationを持つ
	//targetLocation.xなど
	FVector targetLocation;

	FName re_Name;

private:

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
