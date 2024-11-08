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
		UUserWidget* userWidget = UWidgetBlueprintLibrary::Create(GetWorld(), widgetClass, playerController);
		userWidget->AddToViewport(0);

		// MouseCursorを表示
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(playerController);
		playerController->SetShowMouseCursor(false);
	}
}