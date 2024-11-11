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

		if (player->AttackCoolTime != 0) cooltime_attack = (maxCooltime_attack / player->AttackCoolTime) / 10.0f;
		else cooltime_attack = 0.0f;

		if (player->BlinkCoolTime != 0) cooltime_avoid = (maxCooltime_avoid / player->BlinkCoolTime) / 10.0f;
		else cooltime_avoid = 0.0f;
	}

	if (progressBar_cooltime_attack) progressBar_cooltime_attack->SetPercent(cooltime_attack);
	if (progressBar_cooltime_avoid)  progressBar_cooltime_avoid ->SetPercent(cooltime_avoid);
}