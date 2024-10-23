// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/stage/mainmeum/MainMenuHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void AMainMenuHUD::BeginPlay()
{
	// WidgetBlueprintのClassを取得する
	FString Path = TEXT("/Game/Game/stage/mainmeum/title/BPW_Title.BPW_Title_C");
	TSubclassOf<UUserWidget> WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

	// PlayerControllerを取得する
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// WidgetClassとPlayerControllerが取得できたか判定する
	if (WidgetClass && PlayerController)
	{
		// Widgetを作成する
		UUserWidget* UserWidget = UWidgetBlueprintLibrary::Create(GetWorld(), WidgetClass, PlayerController);

		// Viewportに追加する
		UserWidget->AddToViewport(0);

		// MouseCursorを表示する
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, UserWidget, EMouseLockMode::DoNotLock, true, false);
		PlayerController->SetShowMouseCursor(true);
	}
}
