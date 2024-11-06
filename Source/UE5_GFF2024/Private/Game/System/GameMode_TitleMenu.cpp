// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/System/GameMode_TitleMenu.h"
#include "Game/UI/HUD_TitleMenu.h"

AGameMode_TitleMenu::AGameMode_TitleMenu()
{
	HUDClass = AHUD_TitleMenu::StaticClass();
}