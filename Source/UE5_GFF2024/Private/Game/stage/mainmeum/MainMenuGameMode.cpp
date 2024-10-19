// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/stage/mainmeum/MainMenuGameMode.h"
#include "Game/stage/mainmeum/MainMenuHUD.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	HUDClass = AMainMenuHUD::StaticClass();
}
