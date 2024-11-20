// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/System/GameMode_InGame.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Game/Player_Cube.h"
#include "Game/System/GameInstance_GFF2024.h"
#include "Game/UI/HUD_PlayerHUD.h"

AGameMode_InGame::AGameMode_InGame()
{
	DefaultPawnClass = APlayer_Cube::StaticClass();
	HUDClass = AHUD_PlayerHUD::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
}

void AGameMode_InGame::BeginPlay()
{
	Super::BeginPlay();

	//Viewportに配置されたPlayerStartを探す
	const APlayerStart* PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));

	//PlayerStartの位置情報をRespawnの位置として保持する
	SpawnTransform = PlayerStart->GetActorTransform();
}

void AGameMode_InGame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (false) // TODO: 入力->ポーズ画面表示 の実装が必要
	{
		AHUD_PlayerHUD* PlayerHUD = Cast<AHUD_PlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
		if (PlayerHUD && PlayerHUD->userWidget_pauseMenu)
		{
			PlayerHUD->userWidget_pauseMenu->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AGameMode_InGame::KillPlayer(APlayer_Cube* Player)
{
	//Playerを破棄する
	Player->Destroy();

	//GameInstanceを取得する
	UGameInstance_GFF2024* GameInstance = Cast<UGameInstance_GFF2024>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GameInstance)
	{
		//Lifeをデクリメントする
		GameInstance->Life--;

		if (0 <= GameInstance->Life)
		{
			//Respawnを行う
			RespawnPlayer();
		}
		else
		{
			//GameをRestartする
			UE_LOG(LogTemp, Display, TEXT("GameOver"));
			//RestartGame();
		}
	}
}

void AGameMode_InGame::RestartGame()
{

}

void AGameMode_InGame::RespawnPlayer()
{
	//PlayerをSpawnする
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	APlayer_Cube* Player = GetWorld()->SpawnActor<APlayer_Cube>(APlayer_Cube::StaticClass(), SpawnTransform, SpawnInfo);

	//Controllerを設定する
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->Possess(Player);
}
