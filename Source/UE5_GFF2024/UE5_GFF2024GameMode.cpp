// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE5_GFF2024GameMode.h"
#include "UE5_GFF2024Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Game/Player_Box.h"
#include "Game/Player_Cube.h"

AUE5_GFF2024GameMode::AUE5_GFF2024GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	//DefaultPawnClass = APlayer_Box::StaticClass();
	DefaultPawnClass = APlayer_Cube::StaticClass();
}
