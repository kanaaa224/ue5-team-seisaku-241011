// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/4/Enemy4Character.h"

// Sets default values
AEnemy4Character::AEnemy4Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemy4Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy4Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy4Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

