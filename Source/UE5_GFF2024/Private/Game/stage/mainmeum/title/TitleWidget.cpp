// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/stage/mainmeum/title/TitleWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

#include "Kismet/KismetSystemLibrary.h"

void UTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//ButtonPlayのOnClickedに「OnButtonPlayClicked」を関連づける
	ButtonPlay->OnClicked.AddUniqueDynamic(this, &UTitleWidget::OnButtonPlayClicked);

	//ButtonQuitのOnClicked「OnButtonQuitClicked」を関連づける
	ButtonQuit->OnClicked.AddUniqueDynamic(this, &UTitleWidget::OnButtonQuitClicked);
}

void UTitleWidget::OnButtonPlayClicked()
{
	UE_LOG(LogTemp, Log, TEXT("LogMessage:ClickedPlayButton"));
}

void UTitleWidget::OnButtonQuitClicked()
{
	UE_LOG(LogTemp, Log, TEXT("LogMessage:ClickedQuitButton"));
}
