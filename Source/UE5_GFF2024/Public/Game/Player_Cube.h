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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Cube;

	//マテリアルインスタンス
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstanceDynamic* Material_Instance;

	//スプリングアーム
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	//カメラをスムーズにするためのスプリングアーム
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	//カメラ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

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
	//ロックオンのインプットアクション
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LockOnAction;

	//ロックオンのボックスコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* LockOnCollision;

	//ロックオンのボックスコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* AttackCollision;

public:
	//ブリンク用のカーブ
	class UCurveFloat* BlinkCurve;
	//攻撃用のカーブ
	class UCurveFloat* AttackCurve;
	//ノックバック用のカーブ
	class UCurveFloat* KnockBackCurve;
	//起き上がり用のカーブ
	class UCurveFloat* GetUpCurve;

	//ブリンクのタイムラインコンポーネント
	class UTimelineComponent* BlinkTimeline;
	//攻撃のタイムラインコンポーネント
	class UTimelineComponent* AttackTimeline;
	//ノックバックのタイムラインコンポーネント
	class UTimelineComponent* KnockBackTimeline;
	//起き上がりのタイムラインコンポーネント
	class UTimelineComponent* GetUpTimeline;

	//ロックオンの候補
	TArray<class AActor*> LockOnCandidates;

	//ロックオンの対象
	class AActor* LockOnTargetActor;

	//パーティクル
	class UParticleSystem* AttackParticle;

	//ブリンクの初期座標
	FVector BlinkInitLocation;
	//ノックバックの初期座標
	FVector KnockBackInitLocation;
	//ブリンクの前方方向
	FVector BlinkForwardVector;
	//ブリンクの右方向
	FVector BlinkRightVector;
	//ノックバックの前方方向
	FVector KnockBackForwardVector;
	//カメラの衝突の絶対座標
	FVector CameraImpactPoint;

	//ノックバックの初期回転値
	FRotator KnockBackInitRotation;

	//ブリンクのクールタイム
	int BlinkCoolTime;
	//攻撃のクールタイム
	int AttackCoolTime;

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
	//ロックオンのフラグ
	bool LockOnFlg;
	//ロックオンの対処削除フラグ
	bool LockOnRemoveFlg;

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
	//起き上がりのタイムライン更新時に呼ばれる処理
	UFUNCTION()
	void GetUpTimelineUpdate(float Value);


	//ブリンクのタイムライン終了時に呼ばれる処理
	UFUNCTION()
	void BlinkTimelineFinished();
	//攻撃のタイムライン終了時に呼ばれる処理
	UFUNCTION()
	void AttackTimelineFinished();
	//ノックバックのタイムライン終了時に呼ばれる処理
	UFUNCTION()
	void KnockBackTimelineFinished();
	//起き上がりのタイムライン終了時に呼ばれる処理
	UFUNCTION()
	void GetUpTimelineFinished();

	UFUNCTION()
	void OnLockOnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnLockOnCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnAttackCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnAttackCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	//移動処理
	void Move(const FInputActionValue& Value);
	//視点操作処理
	void Look(const FInputActionValue& Value);
	//ブリンク処理
	void Blink(const FInputActionValue& Value);
	//攻撃処理
	void Attack(const FInputActionValue& Value);
	//ロックオン処理
	void LockOn(const FInputActionValue& Value);

	//カメラのコリジョンの処理
	void SmoothCameraCollision();
	//ロックオン後のカメラやプレイヤーの処理
	void LockOnTarget();
	//プレイヤーの透過処理
	void PlayerTransparent();

private:
	//ソートされた配列の最初の要素を取得
	AActor* GetArraySortingFirstElement(TArray<AActor*> Array);

public:
	//CameraBoomを取得
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	//FollowCameraを取得
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};
