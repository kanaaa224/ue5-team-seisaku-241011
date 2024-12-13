// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/Widget_FadeAnimationPanel.h"

void UWidget_FadeAnimationPanel::NativeConstruct()
{
	Super::NativeConstruct();

	speed = 4.0f;

	isShow = false;

	this->SetRenderOpacity(0.0f);
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidget_FadeAnimationPanel::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	float opacity = this->RenderOpacity;

	if (isShow) opacity = FMath::FInterpTo(opacity, 1.0f, InDeltaTime, speed);
	else        opacity = FMath::FInterpTo(opacity, 0.0f, InDeltaTime, speed);

	this->SetRenderOpacity(opacity);

	if (opacity > 0.0f) this->SetVisibility(ESlateVisibility::Visible);
	else                this->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidget_FadeAnimationPanel::set_isShow(bool value)
{
	isShow = value;

	if (value) this->SetVisibility(ESlateVisibility::Visible);
}