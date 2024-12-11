// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/Widget_GameClear.h"
#include "Kismet/GameplayStatics.h"

void UWidget_GameClear::OpenNextLevel()
{
	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() 
		{
			if (!IsValid(this))
			{
				return;
			}
			UGameplayStatics::OpenLevel(GetWorld(), "Level_TitleMenu"); 
		}, 2.f, false);
}
