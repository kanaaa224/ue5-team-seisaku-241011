// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUD_PlayerHUD.generated.h"

UCLASS()
class UE5_GFF2024_API AHUD_PlayerHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite, Category = "UI") UUserWidget* userWidget_playerHUD;
	UPROPERTY(BlueprintReadWrite, Category = "UI") UUserWidget* userWidget_pauseMenu;
};