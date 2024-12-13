// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/Widget_TitleMenu.h"
#include "Game/UI/HUD_TitleMenu.h"
#include "Game/System/GameInstance_GFF2024.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UWidget_TitleMenu::NativeConstruct()
{
	Super::NativeConstruct();

	button_play->OnClicked.AddUniqueDynamic(this, &UWidget_TitleMenu::onButtonPlayClicked);
	button_quit->OnClicked.AddUniqueDynamic(this, &UWidget_TitleMenu::onButtonQuitClicked);
}

void UWidget_TitleMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (!button_play->GetIsEnabled())
	{
		AHUD_TitleMenu* hud = Cast<AHUD_TitleMenu>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());

		if (hud)
		{
			hud->userWidget_fadeAnimationPanel->set_isShow(true);

			if (hud->userWidget_fadeAnimationPanel->GetRenderOpacity() >= 1.0f)
			{
				UGameInstance_GFF2024* GameInstance = Cast<UGameInstance_GFF2024>(UGameplayStatics::GetGameInstance(GetWorld()));

				if (GameInstance) GameInstance->Initialize(); // GameInstance初期化

				UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Stage1Map"))); // レベルをロード
			}
		}
	}
}

void UWidget_TitleMenu::onButtonPlayClicked()
{
	button_play->SetIsEnabled(false);
	button_quit->SetIsEnabled(false);
}

void UWidget_TitleMenu::onButtonQuitClicked()
{
	if (APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
		UKismetSystemLibrary::QuitGame(GetWorld(), playerController, EQuitPreference::Quit, false); // ゲームを終了
	}
}