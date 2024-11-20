// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Widget_PauseMenu.generated.h"

class UButton;

UCLASS()
class UE5_GFF2024_API UWidget_PauseMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> button_close;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> button_title;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> button_quit;

	UFUNCTION()
	void onButtonCloseClicked();

	UFUNCTION()
	void onButtonTitleClicked();

	UFUNCTION()
	void onButtonQuitClicked();
};