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

		if (player->Health > 0.0f && player->Health <= maxHP) hp = player->Health / maxHP;
		else hp = 0.0f;
	}

	if (progressBar)
	{
		progressBar->SetPercent(hp);

		if (hp <= 0.25f)
		{
			FLinearColor color = FLinearColor::Red;
			progressBar->SetFillColorAndOpacity(color);
		}
		else if (hp <= 0.5f)
		{
			FLinearColor color(1.0f, 0.65f, 0.0f, 1.0f);
			progressBar->SetFillColorAndOpacity(color);
		}
		else
		{
			FLinearColor color(0.0f, 0.5f, 1.0f, 1.0f);
			progressBar->SetFillColorAndOpacity(color);
		}
	}
}