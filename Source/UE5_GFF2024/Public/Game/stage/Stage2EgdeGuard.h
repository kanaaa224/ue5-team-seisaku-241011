// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/World.h"
#include "Stage2EgdeGuard.generated.h"

UCLASS()
class UE5_GFF2024_API AStage2EgdeGuard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStage2EgdeGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	/* DiceMehComponent */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StageWall;

	/* 当たり判定用 BoxComponent */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> HitBox;

	/* StaticMeshComponentの向きがわからないため設置 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UArrowComponent> Arrow;

	FName StageEdgeVector[4];

	UMaterialInstanceDynamic* DynamicMaterial;

	bool PlayerIsRange[4];

private:

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
