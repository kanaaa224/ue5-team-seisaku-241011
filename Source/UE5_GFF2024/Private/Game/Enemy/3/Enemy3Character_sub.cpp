// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/3/Enemy3Character_sub.h"
#include "Game/Enemy/3/AIC_Enemy3.h"

AEnemy3Character_sub::AEnemy3Character_sub()
{
	/* AI*/
	AIControllerClass = AAIC_Enemy3::StaticClass();

	/* DefaultSceneRootを作成する */
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	/* SceneComponentをRootComponentに設定する */
	RootComponent = DefaultSceneRoot;
	/* StaticMeshComponentを作成する */
	DiceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	/* StaticMeshをLaodしてStaticMeshComponentのStaticMeshに設定する */
	UStaticMesh* LoadMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Game/enemy/3/Mesh/DiceStaticMesh"));
	/* StaticMeshをStaticMeshComponentに設定する */
	DiceMesh->SetStaticMesh(LoadMesh);
	DiceMesh->SetupAttachment(RootComponent);
}
