// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Widget_PlayerHUD.generated.h"

class UTextBlock;
class USizeBox;

UCLASS()
class UE5_GFF2024_API UWidget_PlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> text_stage_name;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> text_stage_description;

	UPROPERTY(meta = (BindWidget))
	USizeBox* sizeBox_wbp_enemyHP;
};