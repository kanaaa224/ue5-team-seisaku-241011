// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/stage/mainmeum/title/BPW_Title.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

#include "Kismet/KismetSystemLibrary.h"

void UBPW_Title::NativeConstruct()
{
	Super::NativeConstruct();

	// ButtonPlayのOnClickedに「OnButtonPlayClicked」を関連づける
	//ButtonPlay->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnButtonPlayClicked);

	// ButtonQuitのOnClickedに「OnButtonQuitClicked」を関連づける
	//ButtonQuit->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnButtonQuitClicked);
}

void UBPW_Title::OnButtonPlayClicked()
{
	// GameInstanceの変数を初期化する
	

	//開始するStageをLoadする
	//UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("")));
}

void UBPW_Title::OnButtonQuitClicked()
{
	// PlayerControllerを取得する
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		// ゲームを終了する
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
	}
}
