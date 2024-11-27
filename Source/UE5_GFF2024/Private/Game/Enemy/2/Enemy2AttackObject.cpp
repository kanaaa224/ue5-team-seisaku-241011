// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/Enemy/2/Enemy2AttackObject.h"

//StaticMeshComponent
#include "Components/StaticMeshComponent.h"
//CapsuleComponent
#include "Components/CapsuleComponent.h"
//Movement
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemy2AttackObject::AEnemy2AttackObject()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Cubeを作成
	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	CubeMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMeshAsset.Succeeded())
	{
		//メッシュを設定
		CubeMesh->SetStaticMesh(CubeMeshAsset.Object);
		//大きさを設定
		CubeMesh->SetRelativeScale3D(FVector(2.0f, 2.0f, 6.0f));
		//RootComponentに対しての位置を設定
		/*float CubeHeight = 500.0f;
		CubeMesh->SetRelativeLocation(FVector(0.0f, 0.0f, CubeHeight / 2.0f));*/
	}

	//マテリアルをロード
	UMaterialInterface* normalMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Game/enemy/2/Material/M_Normal.M_Normal"));

	if (normalMaterial) {
		CubeMesh->SetMaterial(0, normalMaterial);
	}
}

// Called when the game starts or when spawned
void AEnemy2AttackObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy2AttackObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	secTime += DeltaTime;
}

void AEnemy2AttackObject::SetCreateNumber(int createNum)
{
	createNumber = createNum;
}

