// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Widget_PlayerCooltime.generated.h"

class UProgressBar;

UCLASS()
class UE5_GFF2024_API UWidget_PlayerCooltime : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> progressBar_cooltime_attack;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> progressBar_cooltime_avoid;
};