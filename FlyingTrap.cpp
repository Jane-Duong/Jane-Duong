// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingTrap.h"
#include "Engine/World.h"

// Sets default values
AFlyingTrap::AFlyingTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create Mesh 
	TrapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrapMesh")); 
	RootComponent = TrapMesh; 

	MoveSpeed = 1.0f; 
	AmplitudeZ = 50.0f; 
	AmplitudeY = 50.0f; 
	Frequency = 1.0f; 

	TimeCounter = 0.0f; 

}

// Called when the game starts or when spawned
void AFlyingTrap::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation(); 
}

// Called every frame
void AFlyingTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeCounter += DeltaTime * MoveSpeed; 

	// Create 2D wave 
	float NewZ = StartLocation.Z + FMath::Sin(TimeCounter * Frequency) * AmplitudeZ; // z = sin
	float NewY = StartLocation.Y + FMath::Cos(TimeCounter * Frequency) * AmplitudeY; // y = cos

	float NewX = StartLocation.X; 


	//Update new location 
	SetActorLocation(FVector(NewX, NewY, NewZ)); 

}

