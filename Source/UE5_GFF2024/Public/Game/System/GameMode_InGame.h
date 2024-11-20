// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameMode_InGame.generated.h"

/**
 *
 */
UCLASS()
class UE5_GFF2024_API AGameMode_InGame : public AGameModeBase
{
	GENERATED_BODY()

public:
	FTransform SpawnTransform;

public:
	//コンストラクタ
	AGameMode_InGame();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	//Playerを破棄する
	void KillPlayer(class APlayer_Cube* Player);

	//GameをRestartする
	void RestartGame();

private:
	//PlayerをRespawnする
	void RespawnPlayer();


};
