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

	//ブリンクのインプットアクション
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* BlinkAction;
	//移動のインプットアクション
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
	//視点操作のインプットアクション
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	//攻撃のインプットアクション
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;

public:
	//ブリンク用のカーブ
	class UCurveFloat* BlinkCurve;
	//攻撃用のカーブ
	class UCurveFloat* AttackCurve;
	//ノックバック用のカーブ
	class UCurveFloat* KnockBackCurve;

	//ブリンクのタイムラインコンポーネント
	class UTimelineComponent* BlinkTimeline;
	//攻撃のタイムラインコンポーネント
	class UTimelineComponent* AttackTimeline;
	//ノックバックのタイムラインコンポーネント
	class UTimelineComponent* KnockBackTimeline;

	//ブリンクの初期座標
	FVector BlinkInitLocation;
	//ノックバックの初期座標
	FVector KnockBackInitLocation;

	//ブリンクの前方方向
	FVector BlinkForwardVector;
	//ノックバックの前方方向
	FVector KnockBackForwardVector;

	//ブリンクのクールタイム
	int BlinkCoolTime;

	//タイマー
	float Timer;
	//HP
	float Health;

	//ブリンクのフラグ
	bool BlinkFlg;
	//攻撃のフラグ
	bool AttackFlg;
	//攻撃のダメージフラグ
	bool InflictDamageFlg;
	//無敵のフラグ
	bool InvincibleFlg;
	//ノックバックのフラグ
	bool KnockBackFlg;

public:
	//コンストラクタ
	APlayer_Cube();

protected:
	//ゲーム開始時に一度だけ呼ばれる関数
	virtual void BeginPlay() override;

	//接触時に呼ばれる関数
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)override;

public:	
	//毎フレーム呼ばれる関数
	virtual void Tick(float DeltaTime) override;

	//関数をバインドするために呼ばれる関数
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//ダメージを受ける処理
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)override;

	//ダメージを与える処理
	void InflictDamage(class AActor* Other);
private:
	//ブリンクのタイムライン更新時に呼ばれる処理
	UFUNCTION()
	void BlinkTimelineUpdate(float Value);
	//攻撃のタイムライン更新時に呼ばれる処理
	UFUNCTION()
	void AttackTimelineUpdate(float Value);
	//ノックバックのタイムライン更新時に呼ばれる処理
	UFUNCTION()
	void KnockBackTimelineUpdate(float Value);

	//ブリンクのタイムライン終了時に呼ばれる処理
	UFUNCTION()
	void BlinkTimelineFinished();
	//攻撃のタイムライン終了時に呼ばれる処理
	UFUNCTION()
	void AttackTimelineFinished();
	//ノックバックのタイムライン終了時に呼ばれる処理
	UFUNCTION()
	void KnockBackTimelineFinished();

private:
	//移動処理
	void Move(const FInputActionValue& Value);
	//視点操作処理
	void Look(const FInputActionValue& Value);
	//ブリンク処理
	void Blink(const FInputActionValue& Value);
	//攻撃処理
	void Attack(const FInputActionValue& Value);

public:
	//CameraBoomを取得
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	//FollowCameraを取得
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};
