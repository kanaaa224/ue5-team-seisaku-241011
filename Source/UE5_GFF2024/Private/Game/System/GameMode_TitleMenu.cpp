// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/System/GameMode_TitleMenu.h"
#include "Game/UI/HUD_TitleMenu.h"
#include "Game/Player_Cube.h"

AGameMode_TitleMenu::AGameMode_TitleMenu()
{
	DefaultPawnClass = APlayer_Cube::StaticClass();
	//DefaultPawnClass = nullptr;
	//bStartPlayersAsSpectators = true;
	HUDClass = AHUD_TitleMenu::StaticClass();
}