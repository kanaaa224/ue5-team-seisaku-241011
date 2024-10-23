// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/2/Enemy2Character.h"

// Sets default values
AEnemy2Character::AEnemy2Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemy2Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy2Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy2Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

