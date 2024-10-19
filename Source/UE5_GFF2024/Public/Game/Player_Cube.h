// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Player_Cube.generated.h"

UCLASS()
class UE5_GFF2024_API APlayer_Cube : public ACharacter
{
	GENERATED_BODY()

private:
	//スタティックメッシュ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Cube;

	//スプリングアーム
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	//カメラ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	//ボックスコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComponent;

	//マッピングコンテキスト
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	//ジャンプのインプットアクション
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* BlinkAction;

	//移動のインプットアクション
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	//視点操作のインプットアクション
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

public:
	//ブリンク用のカーブ
	class UCurveFloat* BlinkCurve;

	//ブリンクのタイムラインコンポーネント
	class UTimelineComponent* BlinkTimeline;

	//ブリンクの初期座標
	FVector BlinkInitLocation;

	//タイマー
	float Timer;

	//ブリンクのフラグ
	bool BlinkFlg;

public:
	//コンストラクタ
	APlayer_Cube();

protected:
	//ゲーム開始時に一度だけ呼ばれる関数
	virtual void BeginPlay() override;

public:	
	//毎フレーム呼ばれる関数
	virtual void Tick(float DeltaTime) override;

	//関数をバインドするために呼ばれる関数
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//ブリンクのタイムライン更新時に呼ばれる処理
	UFUNCTION()
	void BlinkTimelineUpdate(float Value);

	//ブリンクのタイムライン終了時に呼ばれる処理
	UFUNCTION()
	void BlinkTimelineFinished();

private:
	//移動処理
	void Move(const FInputActionValue& Value);

	//視点操作処理
	void Look(const FInputActionValue& Value);

	//ブリンク処理
	void Blink(const FInputActionValue& Value);

public:
	//CameraBoomを取得
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	//FollowCameraを取得
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};
