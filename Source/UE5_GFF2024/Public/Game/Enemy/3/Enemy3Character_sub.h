// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/Enemy/3/Enemy3Character.h"
#include "Enemy3Character_sub.generated.h"

class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class UE5_GFF2024_API AEnemy3Character_sub : public AEnemy3Character
{
	GENERATED_BODY()
	
public:

	AEnemy3Character_sub();

private:

	/* DefaultSceneRootの設定 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	/* DiceMehComponent */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> DiceMesh;
};
