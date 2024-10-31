// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/Enemy/Sample/SampleEnemy.h"
#include "Game/Enemy/Commons/PolygonRotationManager.h"

// Sets default values
ASampleEnemy::ASampleEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// StaticMeshComponentを追加し、RootComponentに設定する
	box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// StaticMeshをLaodしてStaticMeshComponentのStaticMeshに設定する
	UStaticMesh* TmpMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LevelPrototyping/Meshes/SM_ChamferCube"));
	box->SetStaticMesh(TmpMesh);

	RootComponent = box;

	Cube = new PolygonRotationManager(CubeVertices, CubeFaces);

	FVector Scale = FVector(100, 100, 100);  // スケール
	FVector Position = GetActorLocation();  // オブジェクトの位置
	Cube->DrawPolyhedronFaceCenters(GetWorld(), *Cube, Scale, Position);

}

// Called when the game starts or when spawned
void ASampleEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASampleEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	delete Cube;
}

// Called every frame
void ASampleEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//スケールと位置を指定して面の中心座標を計算・デバッグ描画
	FVector Scale = FVector(100, 100, 100);  // スケール
	FVector Position = GetActorLocation();  // オブジェクトの位置

	//Cube->SetNextBottom(FVector(0, 1, 0), Scale, Position);
	Cube->SetNextBottom(FVector(1, 1, 0), Scale, Position);

	Cube->SetNewRotationAndLocation(Position);
	FRotator rot1 = Cube->GetNowRotation();
	box->SetRelativeRotation(-1 * rot1);
	SetActorLocation(Cube->GetNewLocation());

	Cube->DrawPolyhedronFaceCenters(GetWorld(), *Cube, Scale, Position);

	for (int i = 0; i < CubeFaces.Num(); i++)
	{
		for (int j = 0; j < CubeFaces[i].Num(); j++)
		{
			int k = j + 1;
			if (k >= 3)
			{
				k = 0;
			}
			FVector Start = CubeVertices[CubeFaces[i][j]] * Scale + Position;
			FVector End = CubeVertices[CubeFaces[i][k]] * Scale + Position;
			DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.1f);
		}
	}
}

 //Called to bind functionality to input
void ASampleEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

