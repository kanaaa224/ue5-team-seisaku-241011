// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/3/Enemy3Character.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Game/Player_Cube.h"
#include "Game/Enemy/3/AIC_Enemy3.h"

// Sets default values
AEnemy3Character::AEnemy3Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	// 視野
	PawnSensingComp->SetPeripheralVisionAngle(60.f);
	// 見える範囲
	PawnSensingComp->SightRadius = 2000;
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemy3Character::OnSeePlayer);
}

// Called when the game starts or when spawned
void AEnemy3Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy3Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy3Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy3Character::OnSeePlayer(APawn* Pawn)
{
	AAIC_Enemy3* AIController = Cast<AAIC_Enemy3>(GetController());
	// プレイヤー
	APlayer_Cube* player = Cast<APlayer_Cube>(Pawn);

	if (AIController && player)
	{
		// AIControllerにプレイヤー情報を設定
		AIController->SetPlayerKey(player);
	}

	// 視野に入ったら画面に"See"と表示
	UKismetSystemLibrary::PrintString(this, "Player::See", true, true, FColor::White, 2.f);
}

