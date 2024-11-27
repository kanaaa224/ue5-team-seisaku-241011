// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/Widget_PlayerCooltime.h"
#include "Game/Player_Cube.h"
#include "Kismet/GameplayStatics.h"

void UWidget_PlayerCooltime::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidget_PlayerCooltime::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	float cooltime_attack = 0.0f;
	float cooltime_avoid  = 0.0f;

	APlayer_Cube* player = Cast<APlayer_Cube>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (player)
	{
		int maxCooltime_attack = 90;
		int maxCooltime_avoid  = 90;

		if (player->AttackCoolTime != 0) cooltime_attack = 1.0 - ((float)player->AttackCoolTime / (float)maxCooltime_attack);
		else cooltime_attack = 1.0f;

		if (player->BlinkCoolTime != 0) cooltime_avoid = 1.0 - ((float)player->BlinkCoolTime / (float)maxCooltime_avoid);
		else cooltime_avoid = 1.0f;
	}

	if (progressBar_cooltime_attack) progressBar_cooltime_attack->SetPercent(FMath::Clamp(cooltime_attack, 0.0f, 1.0f));
	if (progressBar_cooltime_avoid)  progressBar_cooltime_avoid ->SetPercent(FMath::Clamp(cooltime_avoid,  0.0f, 1.0f));
}