// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/System/GameMode_InGame.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Game/Player_Cube.h"
#include "Game/System/GameInstance_GFF2024.h"
#include "Game/UI/HUD_PlayerHUD.h"

#include "Game/UI/Widget_GameClear.h"
#include "Game/UI/Widget_GameOver.h"
#include "Game/UI/Widget_StageClear.h"

AGameMode_InGame::AGameMode_InGame()
{
	DefaultPawnClass = APlayer_Cube::StaticClass();
	HUDClass = AHUD_PlayerHUD::StaticClass();

	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<UUserWidget> FindGameOverWidget(TEXT("/Game/Game/UI/Blueprints/WBP_GameOver"));
	if (FindGameOverWidget.Succeeded())
	{
		GameOverWidget = FindGameOverWidget.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> FindGameClearWidget(TEXT("/Game/Game/UI/Blueprints/WBP_GameClear"));
	if (FindGameClearWidget.Succeeded())
	{
		GameClearWidget = FindGameClearWidget.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> FindStageClearWidget(TEXT("/Game/Game/UI/Blueprints/WBP_StageClear"));
	if (FindStageClearWidget.Succeeded())
	{
		StageClearWidget = FindStageClearWidget.Class;
	}
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
			RestartGame();
		}
	}
}

void AGameMode_InGame::RestartGame()
{			
	//ゲームオーバーのクラスが読み込まれているならウィジェット作成
	if (GameOverWidget)
	{
		UUserWidget* WidgetInstancec = CreateWidget<UUserWidget>(GetWorld(), GameOverWidget);

		//ウィジェットを作成できているならウィジェットを表示
		if (WidgetInstancec)
		{
			WidgetInstancec->AddToViewport();
		}

		UWidget_GameOver* GameOver = Cast<UWidget_GameOver>(GameOverWidget);
		if (GameOver)
		{
			GameOver->OpenNextLevel();
		}
	}
}

void AGameMode_InGame::GameClear()
{
	//ゲームオーバーのクラスが読み込まれているならウィジェット作成
	if (GameClearWidget)
	{
		UUserWidget* WidgetInstancec = CreateWidget<UUserWidget>(GetWorld(), GameClearWidget);

		//ウィジェットを作成できているならウィジェットを表示
		if (WidgetInstancec)
		{
			WidgetInstancec->AddToViewport();
		}

		UWidget_GameOver* GameClear = Cast<UWidget_GameOver>(GameClearWidget);
		if (GameClear)
		{
			GameClear->OpenNextLevel();
		}
	}	
}

void AGameMode_InGame::StageClear()
{
	//ゲームオーバーのクラスが読み込まれているならウィジェット作成
	if (StageClearWidget)
	{
		UUserWidget* WidgetInstancec = CreateWidget<UUserWidget>(GetWorld(), StageClearWidget);

		//ウィジェットを作成できているならウィジェットを表示
		if (WidgetInstancec)
		{
			WidgetInstancec->AddToViewport();
		}

		UWidget_GameOver* StageClear = Cast<UWidget_GameOver>(StageClearWidget);
		if (StageClear)
		{
			StageClear->OpenNextLevel();
		}
	}
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
