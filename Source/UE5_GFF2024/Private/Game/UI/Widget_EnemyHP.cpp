// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/Widget_EnemyHP.h"
#include "Kismet/GameplayStatics.h"
#include "Game/UI/HUD_PlayerHUD.h"

void UWidget_EnemyHP::NativeConstruct()
{
	Super::NativeConstruct();

	//SetVisibility(ESlateVisibility::Collapsed);
}

void UWidget_EnemyHP::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	bool isShow = false;

	float hp = 0.0f;

	FText name = FText::FromString(TEXT(""));

	AHUD_PlayerHUD* hud = Cast<AHUD_PlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());

	if (hud)
	{
		isShow = hud->isShow_enemyHP;

		float maxHP = hud->enemyHP_max;

		hp = hud->enemyHP_current;

		if (hp > 0.0f && hp <= maxHP) hp = hp / maxHP;
		else hp = 0.0f;

		name = hud->enemyName;
	}

	if (isShow) SetVisibility(ESlateVisibility::Visible);
	else        SetVisibility(ESlateVisibility::Collapsed);

	if (progressBar)
	{
		float percent = progressBar->Percent;

		if (percent != hp) {
			percent = FMath::FInterpTo(percent, hp, InDeltaTime, 7.5f);
			progressBar->SetPercent(percent);
		}
	}

	if (text_hp) text_hp->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)(hp * 100))));

	if (text_name) text_name->SetText(name);
}