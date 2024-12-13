// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Game/UI/Widget_FadeAnimationPanel.h"
#include "HUD_TitleMenu.generated.h"

UCLASS()
class UE5_GFF2024_API AHUD_TitleMenu : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UUserWidget* userWidget_titleMenu;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UWidget_FadeAnimationPanel* userWidget_fadeAnimationPanel;
};