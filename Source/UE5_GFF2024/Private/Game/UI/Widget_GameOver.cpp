// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/Widget_GameOver.h"
#include "Kismet/GameplayStatics.h"

void UWidget_GameOver::OpenNextLevel()
{
	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() {UGameplayStatics::OpenLevel(GetWorld(), "Level_TitleMenu"); }, 2.f, false);
}
