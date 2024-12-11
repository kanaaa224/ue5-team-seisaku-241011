// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/Widget_StageClear.h"
#include "Kismet/GameplayStatics.h"

void UWidget_StageClear::OpenNextLevel()
{
	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() {UGameplayStatics::OpenLevel(GetWorld(), "Stage2"); }, 2.f, false);
}
