// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_FadeAnimationPanel.generated.h"

UCLASS()
class UE5_GFF2024_API UWidget_FadeAnimationPanel : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	float speed;

	bool isShow;

	void set_isShow(bool);
};