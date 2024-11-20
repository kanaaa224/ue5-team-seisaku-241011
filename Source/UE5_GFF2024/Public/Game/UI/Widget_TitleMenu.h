// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Widget_TitleMenu.generated.h"

class UButton;

UCLASS()
class UE5_GFF2024_API UWidget_TitleMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> button_play;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> button_quit;

	UFUNCTION()
	void onButtonPlayClicked();

	UFUNCTION()
	void onButtonQuitClicked();
};