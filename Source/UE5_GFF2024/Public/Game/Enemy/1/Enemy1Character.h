// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//ロックオン
#include "../../System/LockOnInterface.h"

#include "Enemy1Character.generated.h"

class PolygonRotationManager;
class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class UE5_GFF2024_API AEnemy1Character : public ACharacter, public ILockOnInterface
{
	GENERATED_BODY()

public:
	AEnemy1Character();

protected:
	virtual void BeginPlay() override;
	//virtual void BeginDestroy() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	virtual void Destroyed() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	/*:::::変数:::::*/
	//体力
	float health;

	bool IsMoving;

	FVector Vector;

	PolygonRotationManager* RotationManager;

	UPROPERTY(VisibleAnywhere, Category = Character, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> box;

	//FTimerHandle TimerHandle;

public:
	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

public:
	void ApplyDamage(AActor* Other);

	//ダメージを受ける処理
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)override;
public:
	void MoveProcess();

	void SetVector(FVector v)
	{
		Vector = v;
	}
	void SetIsMoving(bool b)
	{
		IsMoving = b;
	}

	PolygonRotationManager* GetPolygonRotationManager()
	{
		return RotationManager;
	}
public:
	//定義
	// 立方体の頂点を定義（中心が原点、1辺の長さが2の立方体と仮定）
	TArray<FVector> CubeVertices = {
		FVector(-1, -1, -1), FVector(1, -1, -1), FVector(1, 1, -1), FVector(-1, 1, -1),  // 下面
		FVector(-1, -1,  1), FVector(1, -1,  1), FVector(1,  1,  1), FVector(-1,  1,  1)   // 上面
	};

	// 立方体の6つの面を定義（各面は頂点インデックスのリスト）
	TArray<TArray<int32>> CubeFaces = {
		{0, 1, 2, 3},  // 底面
		{4, 5, 6, 7},  // 上面
		{0, 1, 5, 4},  // 前面
		{2, 3, 7, 6},  // 後面
		{0, 3, 7, 4},  // 左面
		{1, 2, 6, 5}   // 右面
	};

	//ここから正二十面体関連
	const float phi = (1.0 + FMath::Sqrt(5.0)) / 2.0;
	const double A = 1.0;
	const double B = 1.0 / phi;

	//TArray<FVector> CubeVertices = {
	//	FVector(-1,  phi, 0), FVector(1,  phi, 0),
	//	FVector(-1, -phi, 0), FVector(1, -phi, 0),

	//	FVector(0, -1,  phi), FVector(0,  1,  phi),
	//	FVector(0, -1, -phi), FVector(0,  1, -phi),

	//	FVector(phi, 0, -1), FVector(phi, 0,  1),
	//	FVector(-phi, 0, -1), FVector(-phi, 0,  1)
	//};

	//// 立方体の6つの面を定義（各面は頂点インデックスのリスト）
	//TArray<TArray<int32>> CubeFaces = {
	//	{0, 11, 5}, {0, 5, 1}, {0, 1, 7}, {0, 7, 10}, {0, 10, 11},
	//	{1, 5, 9}, {5, 11, 4}, {11, 10, 2}, {10, 7, 6}, {7, 1, 8},
	//	{3, 9, 4}, {3, 4, 2}, {3, 2, 6}, {3, 6, 8}, {3, 8, 9},
	//	{4, 9, 5}, {2, 4, 11}, {6, 2, 10}, {8, 6, 7}, {9, 8, 1}
	//};

	TArray<FVector> RotVertices;

	float a = 0;

	// 頂点を回転させる関数
	//FVector RotateVertex(const FVector& Vertex, float AngleX, float AngleY, float AngleZ) {
	//	// X軸回転
	//	FVector Rotated = FVector(
	//		Vertex.X,
	//		Vertex.Y * FMath::Cos(AngleX) - Vertex.Z * FMath::Sin(AngleX),
	//		Vertex.Y * FMath::Sin(AngleX) + Vertex.Z * FMath::Cos(AngleX)
	//	);

	//	// Y軸回転
	//	Rotated = FVector(
	//		Rotated.X * FMath::Cos(AngleY) + Rotated.Z * FMath::Sin(AngleY),
	//		Rotated.Y,
	//		-Rotated.X * FMath::Sin(AngleY) + Rotated.Z * FMath::Cos(AngleY)
	//	);

	//	// Z軸回転
	//	Rotated = FVector(
	//		Rotated.X * FMath::Cos(AngleZ) - Rotated.Y * FMath::Sin(AngleZ),
	//		Rotated.X * FMath::Sin(AngleZ) + Rotated.Y * FMath::Cos(AngleZ),
	//		Rotated.Z
	//	);

	//	return Rotated;
	//}

	
	float Delta;
	void MoveTick(FVector TargetLocation1, float speed)
	{
		FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation1, Delta, speed);
		SetActorLocation(NewLocation);
	}

	FVector TargetLocation = { -1, -1, -10000 };
	FVector OldTargetLocation = { -1, -1, -10000 };
	float Speed;

	void SetSpeed(float s)
	{
		Speed = s;
	}

	// 攻撃コリジョン用のコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TArray<UBoxComponent*> AttackCollisions;

	int32 BottomCollisionNumber;

	// 攻撃メソッド
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Attack();

	// ダメージを与える関数
	UFUNCTION()
	void OnAttackHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	// ダメージ量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Damage = 20.0f;

	//0移動1落下2タックル
	int32 AttackState;


	void GetBottomNumber();

	bool IsAttacking;

	bool IsAttackCoolTime;

	//ロックオン
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Widget, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* LockOnMarkerWidget;

	//ロックオンの有効フラグを設定する
	virtual void SetLockOnEnable_Implementation(bool LockOnFlg)override;



	FVector MoveDirection;


	bool IsDestroy;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TSoftObjectPtr<UWorld> LoadLevel;


	UPROPERTY()
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY()
	UUserWidget* WidgetInstance;

	// マテリアル変更関数
	UFUNCTION(BlueprintCallable, Category = "Material")
	void ChangeMaterial(UMaterialInterface* NewMaterial);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* HitMaterial;
};
