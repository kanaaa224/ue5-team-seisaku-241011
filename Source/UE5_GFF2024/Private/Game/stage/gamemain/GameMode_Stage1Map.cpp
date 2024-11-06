// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/stage/gamemain/GameMode_Stage1Map.h"
#include "Game/Player_Cube.h"
#include "Game/UI/HUD_PlayerHUD.h"

AGameMode_Stage1Map::AGameMode_Stage1Map()
{
	DefaultPawnClass = APlayer_Cube::StaticClass();
	HUDClass = AHUD_PlayerHUD::StaticClass();
}
