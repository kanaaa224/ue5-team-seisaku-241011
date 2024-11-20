// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUD_TitleMenu.generated.h"

UCLASS()
class UE5_GFF2024_API AHUD_TitleMenu : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	//virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UUserWidget* userWidget;
};