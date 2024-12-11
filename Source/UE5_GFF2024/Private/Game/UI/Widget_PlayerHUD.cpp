// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/Widget_PlayerHUD.h"
#include "Game/UI/HUD_PlayerHUD.h"
#include "Kismet/GameplayStatics.h"

void UWidget_PlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();

	if (sizeBox_wbp_enemyHP) sizeBox_wbp_enemyHP->SetRenderOpacity(0.0f);
}

void UWidget_PlayerHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	FText stageName        = FText::FromString(TEXT(""));
	FText stageDescription = FText::FromString(TEXT(""));

	bool isShow_enemyHP = false;

	AHUD_PlayerHUD* hud = Cast<AHUD_PlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());

	if (hud)
	{
		stageName        = hud->stageName;
		stageDescription = hud->stageDescription;

		isShow_enemyHP = hud->isShow_enemyHP;
	}

	if (text_stage_name)        text_stage_name       ->SetText(stageName);
	if (text_stage_description) text_stage_description->SetText(stageDescription);

	if (sizeBox_wbp_enemyHP)
	{
		float opacity = sizeBox_wbp_enemyHP->RenderOpacity;

		if (isShow_enemyHP) opacity = FMath::FInterpTo(opacity, 1.0f, InDeltaTime, 2.5f);
		else                opacity = FMath::FInterpTo(opacity, 0.0f, InDeltaTime, 2.5f);

		sizeBox_wbp_enemyHP->SetRenderOpacity(opacity);

		if (opacity > 0.0f) sizeBox_wbp_enemyHP->SetVisibility(ESlateVisibility::Visible);
		else                sizeBox_wbp_enemyHP->SetVisibility(ESlateVisibility::Collapsed);
	}
}