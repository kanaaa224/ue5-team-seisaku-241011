// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/Widget_PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Game/UI/HUD_PlayerHUD.h"

void UWidget_PlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidget_PlayerHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	FText stageName        = FText::FromString(TEXT(""));
	FText stageDescription = FText::FromString(TEXT(""));

	AHUD_PlayerHUD* hud = Cast<AHUD_PlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());

	if (hud)
	{
		stageName        = hud->stageName;
		stageDescription = hud->stageDescription;
	}

	if (text_stage_name)        text_stage_name       ->SetText(stageName);
	if (text_stage_description) text_stage_description->SetText(stageDescription);
}