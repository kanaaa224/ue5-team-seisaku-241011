// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/HUD_TitleMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void AHUD_TitleMenu::BeginPlay()
{
	// PlayerControllerを取得
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (true)
	{
		// WidgetBlueprintのClassを取得
		FString path = TEXT("/Game/Game/UI/Blueprints/WBP_TitleMenu.WBP_TitleMenu_C");
		TSubclassOf<UUserWidget> widgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*path)).LoadSynchronous();

		if (widgetClass && playerController)
		{
			// Widgetを作成し、Viewportに追加
			userWidget_titleMenu = UWidgetBlueprintLibrary::Create(GetWorld(), widgetClass, playerController);
			userWidget_titleMenu->AddToViewport(0);
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
	}

	if (playerController)
	{
		// InputModeをゲーム&UIにし、MouseCursorを表示
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(playerController);
		playerController->SetShowMouseCursor(true);
	}
}