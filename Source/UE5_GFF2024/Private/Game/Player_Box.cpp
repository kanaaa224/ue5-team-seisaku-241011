// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Player_Box.h"

// Sets default values
APlayer_Box::APlayer_Box()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayer_Box::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayer_Box::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayer_Box::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

