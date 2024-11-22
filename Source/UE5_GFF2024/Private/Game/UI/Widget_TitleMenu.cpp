// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/Widget_TitleMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UWidget_TitleMenu::NativeConstruct()
{
	Super::NativeConstruct();

	button_play->OnClicked.AddUniqueDynamic(this, &UWidget_TitleMenu::onButtonPlayClicked);
	button_quit->OnClicked.AddUniqueDynamic(this, &UWidget_TitleMenu::onButtonQuitClicked);
}

void UWidget_TitleMenu::onButtonPlayClicked()
{
	// レベルをロード
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Stage1Map")));
}

void UWidget_TitleMenu::onButtonQuitClicked()
{
	// PlayerControllerを取得
	if (APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		// ゲームを終了
		UKismetSystemLibrary::QuitGame(GetWorld(), playerController, EQuitPreference::Quit, false);
	}
}