// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/stage/mainmeum/title/TitlePlayerMannequin.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ATitlePlayerMannequin::ATitlePlayerMannequin()
{
    // This actor will tick every frame. You can disable this if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create the static mesh component
    BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
    RootComponent = BoxMesh;

    // Set the static mesh to a default cube
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxAsset(TEXT("/Engine/BasicShapes/Cube"));
    if (BoxAsset.Succeeded())
    {
        BoxMesh->SetStaticMesh(BoxAsset.Object);
    }

    // Set the material color to white
    BoxMesh->SetMaterial(0, UMaterial::GetDefaultMaterial(MD_Surface));

    // Set a default white color for the mesh (Optional step if you want to ensure the color is pure white)
    //BoxMesh->SetVectorParameterValueOnMaterials(TEXT("BaseColor"), FLinearColor::White);
}

// Called when the game starts or when spawned
void ATitlePlayerMannequin::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATitlePlayerMannequin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

