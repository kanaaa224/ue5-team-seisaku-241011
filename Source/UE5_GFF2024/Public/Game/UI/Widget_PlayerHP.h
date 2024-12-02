// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Widget_PlayerHP.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class UE5_GFF2024_API UWidget_PlayerHP : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> progressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> text;
};