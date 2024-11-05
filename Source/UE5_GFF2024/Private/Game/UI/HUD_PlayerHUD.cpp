// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/HUD_PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void AHUD_PlayerHUD::BeginPlay()
{
	// プレイヤーコントローラーを取得
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (playerController)
	{
		// マウスカーソルを非表示に設定
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(playerController);
		playerController->SetShowMouseCursor(false);
	}
}