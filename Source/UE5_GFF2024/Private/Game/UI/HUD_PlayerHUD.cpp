// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/HUD_PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void AHUD_PlayerHUD::BeginPlay()
{
	// WidgetBlueprintのClassを取得
	FString path = TEXT("/Game/Game/UI/Blueprints/WBP_PlayerHUD.WBP_PlayerHUD_C");
	TSubclassOf<UUserWidget> widgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*path)).LoadSynchronous();

	// PlayerControllerを取得
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (widgetClass && playerController)
	{
		// Widgetを作成し、Viewportに追加
		userWidget_playerHUD = UWidgetBlueprintLibrary::Create(GetWorld(), widgetClass, playerController);
		userWidget_playerHUD->AddToViewport(0);
	}

	// WidgetBlueprintのClassを取得
	path = TEXT("/Game/Game/UI/Blueprints/WBP_PauseMenu.WBP_PauseMenu_C");
	widgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*path)).LoadSynchronous();

	if (widgetClass && playerController)
	{
		// Widgetを作成し、Viewportに追加
		userWidget_pauseMenu = UWidgetBlueprintLibrary::Create(GetWorld(), widgetClass, playerController);
		userWidget_pauseMenu->AddToViewport(0);
	}

	if (playerController)
	{
		// InputModeをゲームのみにし、MouseCursorを非表示
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(playerController);
		playerController->SetShowMouseCursor(false);
	}
}