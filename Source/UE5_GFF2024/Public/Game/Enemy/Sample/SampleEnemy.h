// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SampleEnemy.generated.h"

class UStaticMeshComponent;
class PolygonRotationManager;

UCLASS()
class UE5_GFF2024_API ASampleEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASampleEnemy(); 

	UPROPERTY(VisibleAnywhere, Category = Character, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> box;

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

	const float phi = (1.0 + FMath::Sqrt(5.0)) / 2.0;

	// 立方体の頂点を定義（中心が原点、1辺の長さが2の立方体と仮定）
	//TArray<FVector> CubeVertices = {
	///*	FVector(-1,  phi, 0), FVector(1,  phi, 0),
	//	FVector(-1, -phi, 0), FVector(1, -phi, 0),

	//	FVector(0, -1,  phi), FVector(0,  1,  phi),
	//	FVector(0, -1, -phi), FVector(0,  1, -phi),

	//	FVector(phi, 0, -1), FVector(phi, 0,  1),
	//	FVector(-phi, 0, -1), FVector(-phi, 0,  1)*/

	//FVector(0,  phi,  1),    // 0
	//FVector(0,  phi, -1),    // 1
	//FVector(phi,  1,  0),   // 2
	//FVector(phi, -1,  0),   // 3
	//FVector(0, -phi, -1),    // 4
	//FVector(0, -phi,  1),    // 5
	//FVector(-phi, -1,  0),   // 6
	//FVector(-phi,  1,  0),   // 7
	//FVector(1, 0, -phi),     // 8
	//FVector(-1, 0, -phi),    // 9
	//FVector(-1, 0,  phi),    // 10
	//FVector(1, 0,  phi)      // 11
	//};

	//// 立方体の6つの面を定義（各面は頂点インデックスのリスト）
	//TArray<TArray<int32>> CubeFaces = {
	//	/*{0, 11, 5}, {0, 5, 1}, {0, 1, 7}, {0, 7, 10}, {0, 10, 11},
	//	{1, 5, 9}, {5, 11, 4}, {11, 10, 2}, {10, 7, 6}, {7, 1, 8},
	//	{3, 9, 4}, {3, 4, 2}, {3, 2, 6}, {3, 6, 8}, {3, 8, 9},
	//	{4, 9, 5}, {2, 4, 11}, {6, 2, 10}, {8, 6, 7}, {9, 8, 1}*/

	//	{0, 1, 2}, {0, 2, 11}, {0, 11, 5}, {0, 5, 10}, {0, 10, 1},   // 面0〜4
	//{1, 10, 7}, {1, 7, 8}, {1, 8, 2}, {2, 8, 3}, {2, 3, 11},     // 面5〜9
	//{3, 8, 4}, {3, 4, 5}, {5, 11, 3}, {4, 8, 9}, {4, 9, 10},     // 面10〜14
	//{4, 10, 5}, {6, 7, 10}, {6, 10, 5}, {6, 5, 9}, {6, 9, 8}     // 面15〜19
	//};

	PolygonRotationManager* Cube;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
