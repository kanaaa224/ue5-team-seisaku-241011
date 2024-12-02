// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/Widget_PlayerLives.h"
#include "Game/System/GameInstance_GFF2024.h"
#include "Kismet/GameplayStatics.h"

void UWidget_PlayerLives::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidget_PlayerLives::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UGameInstance_GFF2024* GameInstance = Cast<UGameInstance_GFF2024>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GameInstance && GameInstance->Life)
	{
		int32 lives = GameInstance->Life; // 残機数

		if(text_lives) text_lives->SetText(FText::FromString(FString::Printf(TEXT("%d"), lives))); // if(text_lives) text_lives->SetText(FText::Format(NSLOCTEXT("UI", "LivesRemaining", "{0}"), FText::AsNumber(lives)));
		
		/*int32 maxLives = 2; // 残機最大数

		// 残機の最大数に応じて残機の表示数を変える
		if (maxLives < 3) {
			if (image_life_3) image_life_3->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (maxLives < 2) {
			if (image_life_2) image_life_2->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (maxLives < 1) {
			if (image_life_1) image_life_1->SetVisibility(ESlateVisibility::Collapsed);
		}

		// 残機数に応じて半透明にする
		if (maxLives <= 3 && lives < 3) {
			if (image_life_3) image_life_3->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.1f));
		}

		if (maxLives <= 2 && lives < 2) {
			if (image_life_2) image_life_2->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.1f));
		}

		if (maxLives <= 1 && lives < 1) {
			if (image_life_1) image_life_1->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.1f));
		}*/
	}
}