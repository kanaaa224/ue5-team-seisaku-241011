// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/HUD_PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

AHUD_PlayerHUD::AHUD_PlayerHUD()
{
	stageName        = FText::FromString(TEXT(""));
	stageDescription = FText::FromString(TEXT(""));

	isShow_enemyHP = false;

	enemyHP_current = 0.0f;
	enemyHP_max     = 0.0f;
	enemyName       = FText::FromString(TEXT(""));
}

void AHUD_PlayerHUD::BeginPlay()
{
	// PlayerControllerを取得
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (true)
	{
		// WidgetBlueprintのClassを取得
		FString path = TEXT("/Game/Game/UI/Blueprints/WBP_PlayerHUD.WBP_PlayerHUD_C");
		TSubclassOf<UUserWidget> widgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*path)).LoadSynchronous();

		if (widgetClass && playerController)
		{
			// Widgetを作成し、Viewportに追加
			userWidget_playerHUD = UWidgetBlueprintLibrary::Create(GetWorld(), widgetClass, playerController);
			userWidget_playerHUD->AddToViewport(0);
		}
	}

	if (true)
	{
		// WidgetBlueprintのClassを取得
		FString path = TEXT("/Game/Game/UI/Blueprints/WBP_PauseMenu.WBP_PauseMenu_C");
		TSubclassOf<UUserWidget> widgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*path)).LoadSynchronous();

		if (widgetClass && playerController)
		{
			// Widgetを作成し、Viewportに追加
			userWidget_pauseMenu = UWidgetBlueprintLibrary::Create(GetWorld(), widgetClass, playerController);
			userWidget_pauseMenu->AddToViewport(0);
		}
	}

	if (true)
	{
		// WidgetBlueprintのClassを取得
		FString path = TEXT("/Game/Game/UI/Blueprints/WBP_FadeAnimationPanel.WBP_FadeAnimationPanel_C");
		TSubclassOf<UWidget_FadeAnimationPanel> widgetClass = TSoftClassPtr<UWidget_FadeAnimationPanel>(FSoftObjectPath(*path)).LoadSynchronous();

		if (widgetClass && playerController)
		{
			// Widgetを作成し、Viewportに追加
			userWidget_fadeAnimationPanel = CreateWidget<UWidget_FadeAnimationPanel>(GetWorld(), widgetClass);
			userWidget_fadeAnimationPanel->AddToViewport(0);
		}

		if (userWidget_fadeAnimationPanel)
		{
			userWidget_fadeAnimationPanel->SetRenderOpacity(1.0f);
			userWidget_fadeAnimationPanel->SetVisibility(ESlateVisibility::Visible);
		}
	}

	if (playerController)
	{
		// InputModeをゲームのみにし、MouseCursorを非表示
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(playerController);
		playerController->SetShowMouseCursor(false);
	}
}

void AHUD_PlayerHUD::set_isShow_enemyHP(bool value)
{
	isShow_enemyHP = value;
}

void AHUD_PlayerHUD::set_enemyHP(float value)
{
	enemyHP_current = value;
}

void AHUD_PlayerHUD::set_enemyMaxHP(float value)
{
	enemyHP_max = value;
}

void AHUD_PlayerHUD::set_enemyName(FText value)
{
	enemyName = value;
}