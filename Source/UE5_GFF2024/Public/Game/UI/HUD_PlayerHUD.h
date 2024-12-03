// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUD_PlayerHUD.generated.h"

UCLASS()
class UE5_GFF2024_API AHUD_PlayerHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite, Category = "UI") UUserWidget* userWidget_playerHUD;
	UPROPERTY(BlueprintReadWrite, Category = "UI") UUserWidget* userWidget_pauseMenu;

public:
	bool isShow_enemyHP = false;

	float enemyHP_current = 0.0f;
	float enemyHP_max     = 0.0f;

	FText enemyName = FText::FromString(TEXT(""));

	void set_isShow_enemyHP(bool); // 敵のHPゲージを表示するか

	void set_enemyHP(float);    // 敵のHPを設定
	void set_enemyMaxHP(float); // 敵の最大HPを設定
	void set_enemyName(FText);  // 敵の名前を設定
};