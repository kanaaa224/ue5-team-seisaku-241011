// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/Widget_PauseMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UWidget_PauseMenu::NativeConstruct()
{
	Super::NativeConstruct();

	button_close->OnClicked.AddUniqueDynamic(this, &UWidget_PauseMenu::onButtonCloseClicked);
	button_title->OnClicked.AddUniqueDynamic(this, &UWidget_PauseMenu::onButtonTitleClicked);
	button_quit ->OnClicked.AddUniqueDynamic(this, &UWidget_PauseMenu::onButtonQuitClicked);

	bIsFocusable = true;

	SetVisibility(ESlateVisibility::Collapsed);
}

void UWidget_PauseMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	switch (GetVisibility())
	{
		case ESlateVisibility::Collapsed:
		{
			// マウスカーソルを非表示にし、入力モードを Gameのみ に設定
			if (APlayerController* playerController = GetOwningPlayer())
			{
				if (playerController->bShowMouseCursor)
				{
					playerController->bShowMouseCursor = false;

					FInputModeGameOnly InputMode;
					playerController->SetInputMode(InputMode);
				}
			}

			break;
		}

		case ESlateVisibility::Visible:
		{
			// マウスカーソルを表示し、入力モードを UIのみ に設定
			if (APlayerController* playerController = GetOwningPlayer())
			{
				if (!playerController->bShowMouseCursor)
				{
					playerController->bShowMouseCursor = true;

					FInputModeUIOnly InputMode;
					InputMode.SetWidgetToFocus(TakeWidget());
					playerController->SetInputMode(InputMode);
				}
			}

			break;
		}
	}
}

void UWidget_PauseMenu::onButtonCloseClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UWidget_PauseMenu::onButtonTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Level_TitleMenu")));
}

void UWidget_PauseMenu::onButtonQuitClicked()
{
	if (APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
		UKismetSystemLibrary::QuitGame(GetWorld(), playerController, EQuitPreference::Quit, false);
	}
}