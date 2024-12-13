// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/Widget_TitleMenu.h"
#include "Game/UI/HUD_TitleMenu.h"
#include "Game/System/GameInstance_GFF2024.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EnhancedInputComponent.h"

void UWidget_TitleMenu::NativeConstruct()
{
	Super::NativeConstruct();

	button_play->OnClicked.AddUniqueDynamic(this, &UWidget_TitleMenu::onButtonPlayClicked);
	button_quit->OnClicked.AddUniqueDynamic(this, &UWidget_TitleMenu::onButtonQuitClicked);

	UpAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_Title_Up"));
	DownAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_Title_Down"));
	SelectAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_Attack"));

	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInput->BindAction(UpAction, ETriggerEvent::Triggered, this, &UWidget_TitleMenu::NavigateUp);
			EnhancedInput->BindAction(DownAction, ETriggerEvent::Triggered, this, &UWidget_TitleMenu::NavigateDown);
			EnhancedInput->BindAction(SelectAction, ETriggerEvent::Triggered, this, &UWidget_TitleMenu::Select);
		}
	}
	UpdateSelection();
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

void UWidget_TitleMenu::NavigateUp(const FInputActionValue& Value)
{
	CurrentSelection = (CurrentSelection > 0) ? CurrentSelection - 1 : 1;
	UpdateSelection();
}

void UWidget_TitleMenu::NavigateDown(const FInputActionValue& Value)
{
	CurrentSelection = (CurrentSelection < 1) ? CurrentSelection + 1 : 0;
	UpdateSelection();
}

void UWidget_TitleMenu::Select(const FInputActionValue& Value)
{
	if (CurrentSelection == 0)
	{
		button_play->OnClicked.Broadcast();
	}
	else if (CurrentSelection == 1)
	{
		button_quit->OnClicked.Broadcast();
	}
}

void UWidget_TitleMenu::UpdateSelection()
{
	//// ボタンのデフォルトスタイル
	//FButtonStyle DefaultStyle = button_play->WidgetStyle;
	//DefaultStyle.Normal.TintColor = FSlateColor(FLinearColor::White);
	//DefaultStyle.Hovered.TintColor = FSlateColor(FLinearColor::Gray);

	//// ボタンの選択スタイル
	//FButtonStyle SelectedStyle = button_play->WidgetStyle;
	//SelectedStyle.Normal.TintColor = FSlateColor(FLinearColor::Yellow);
	//SelectedStyle.Hovered.TintColor = FSlateColor(FLinearColor::Yellow);

	if (CurrentSelection == 0)
	{
		/*button_play->SetStyle(SelectedStyle);
		button_quit->SetStyle(DefaultStyle);*/

		button_play->SetFocus();
	}
	else if (CurrentSelection == 1)
	{
		/*button_play->SetStyle(DefaultStyle);
		button_quit->SetStyle(SelectedStyle);*/

		button_quit->SetFocus();
	}
}