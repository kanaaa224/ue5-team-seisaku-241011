// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/Widget_PlayerHP.h"
#include "Game/Player_Cube.h"
#include "Kismet/GameplayStatics.h"

void UWidget_PlayerHP::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidget_PlayerHP::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	float hp = 0.0f;

	APlayer_Cube* player = Cast<APlayer_Cube>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (player)
	{
		float maxHP = 100.0f;

		hp = player->Health;

		if (hp > 0.0f && hp <= maxHP) hp = hp / maxHP;
		else hp = 0.0f;
	}

	if (progressBar)
	{
		float percent = progressBar->Percent;

		if (percent != hp) {
			percent = FMath::FInterpTo(percent, hp, InDeltaTime, 7.5f);
			progressBar->SetPercent(percent);
		}

		FLinearColor colorA, colorB;

		if (hp <= 0.25f) {
			colorA = FLinearColor(1.0f, 0.0f, 0.0f, 0.5f);
			colorB = FLinearColor(1.0f, 0.0f, 0.0f, 0.1f);
		}
		else if (hp <= 0.5f) {
			colorA = FLinearColor(1.0f, 0.2f, 0.0f, 0.5f);
			colorB = FLinearColor(1.0f, 0.2f, 0.0f, 0.1f);
		}
		else {
			colorA = FLinearColor(0.0f, 1.0f, 0.0f, 0.5f);
			colorB = FLinearColor(0.0f, 1.0f, 0.0f, 0.1f);
		}

		progressBar->SetFillColorAndOpacity(colorA);
		progressBar->WidgetStyle.BackgroundImage.TintColor = FSlateColor(colorB);
	}

	if (text) text->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)(hp * 100))));
}