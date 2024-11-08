// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Widget_PlayerLives.generated.h"

class UImage;

UCLASS()
class UE5_GFF2024_API UWidget_PlayerLives : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> image_life_1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> image_life_2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> image_life_3;
};