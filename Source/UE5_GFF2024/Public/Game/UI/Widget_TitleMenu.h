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

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> button_play;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> button_quit;

	UFUNCTION()
	void onButtonPlayClicked();

	UFUNCTION()
	void onButtonQuitClicked();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* UpAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DownAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SelectAction;

	// アクションバインド
	void NavigateUp(const FInputActionValue& Value);
	void NavigateDown(const FInputActionValue& Value);
	void Select(const FInputActionValue& Value);

	// 現在の選択状態を追跡
	int32 CurrentSelection = 0;

	void UpdateSelection();
};