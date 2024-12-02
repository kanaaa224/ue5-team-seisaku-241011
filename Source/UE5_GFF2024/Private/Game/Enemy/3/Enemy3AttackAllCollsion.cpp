// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/3/Enemy3AttackAllCollsion.h"
#include "Engine/DamageEvents.h"
#include "Game/Player_Cube.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Game/Enemy/3/Enemy3Character.h"
#include "Game/Enemy/3/AIC_Enemy3.h"
#include "AIController.h"

// Sets default values
AEnemy3AttackAllCollsion::AEnemy3AttackAllCollsion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* BoxComponentを作成する */
	AttackCollisionMesh = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	/* RootComponentを子クラスのBoxComponentに設定する */
	AttackCollisionMesh->SetupAttachment(RootComponent);

	/* Enemy3Attack用のCollision OnComponentBeginOverlapをBindする */
	AttackCollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &AEnemy3AttackAllCollsion::OnBoxBeginOverlap);
	/* Enemy3Attack用のCollision OnComponentEndOverlapをBindする */
	AttackCollisionMesh->OnComponentEndOverlap.AddDynamic(this, &AEnemy3AttackAllCollsion::OnBoxEndOverlap);
}

// Called when the game starts or when spawned
void AEnemy3AttackAllCollsion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy3AttackAllCollsion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy3AttackAllCollsion::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AEnemy3AttackAllCollsion::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

