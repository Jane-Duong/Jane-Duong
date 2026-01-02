// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicOrbit.h"
#include "TimerManager.h"
#include "Engine/World.h"


// Sets default values
AMagicOrbit* AMagicOrbit::MasterOrbInstance = nullptr; 

AMagicOrbit::AMagicOrbit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Mesh 
	OrbMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OrbitMesh")); 
	RootComponent = OrbMesh; 

	// Default Movement values 
	MoveSpeed = 1.0f; 
	Radius = 200.0f; 
	CurrentAngle = 0.0f; 

	// Default Spawn 
	bIsMasterOrbit = false; 
	SpawnInterval = 3.0f; 
	MinSpawnDistance = 300.0f; 
	MaxSpawnDistance = 800.0f; 
	MaxSpawnedOrbits = 5;

}

// Called when the game starts or when spawned
void AMagicOrbit::BeginPlay()
{
	Super::BeginPlay();

	OriginLocation = GetActorLocation(); 

	// First orbit should be the master 

	if (MasterOrbInstance == nullptr)
	{
		MasterOrbInstance = this; 
		bIsMasterOrbit = true; 
	}
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AMagicOrbit::HandleSpawnTimer, SpawnInterval, true); //start timer spawn
}

// Called every frame
void AMagicOrbit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CurrentAngle += MoveSpeed * DeltaTime; 

	float X = OriginLocation.X + FMath::Cos(CurrentAngle) * Radius; 
	float Y = OriginLocation.Y + FMath::Sin(CurrentAngle) * Radius; 

	SetActorLocation(FVector(X, Y, OriginLocation.Z)); 
}
void AMagicOrbit::HandleSpawnTimer()
{ 
	if (bSpawningPhase)
	{
		if (SpawnedOrbits.Num() < MaxSpawnedOrbits) // have space for spawn
		{
			SpawnOrbit(); 
			if (SpawnedOrbits.Num() >= MaxSpawnedOrbits)
			{
				bSpawningPhase = false; // if max quantity is reached -> destroy 
			}
		}
	}
	else
	{
		if (SpawnedOrbits.Num() > 0)
		{
			DestroyRamdomOrbit();
			if (SpawnedOrbits.Num() == 0) // if don't have orbit left => back to spawn 
			{
				bSpawningPhase = true;
			}
		}
	}
}
void AMagicOrbit::SpawnOrbit()
{
	UWorld* World = GetWorld(); 
	
	// random angle and distance
	float Angle = FMath::RandRange(0.f, 2.f * PI); 
	float Distance = FMath::RandRange(MinSpawnDistance, MaxSpawnDistance); 

	FVector SpawnLocation = GetActorLocation();
	SpawnLocation.X += FMath::Cos(Angle) * Distance; 
	SpawnLocation.Y += FMath::Sin(Angle) * Distance; 

	// Spawn new orbit 

	AMagicOrbit* NewOrbit = World->SpawnActor<AMagicOrbit>(AMagicOrbit::StaticClass(), SpawnLocation, FRotator::ZeroRotator);

	if (NewOrbit)
	{
		NewOrbit->MoveSpeed = FMath::RandRange(0.5f, 2.0f); // random speed and radius for child orbit
		NewOrbit->Radius = FMath::RandRange(100.f, 300.f); 

		NewOrbit->bIsMasterOrbit = false; 

		SpawnedOrbits.Add(NewOrbit); 

		ShowDebugMessage(FString::Printf(TEXT("A new magic orbit was spawned at %s"), *SpawnLocation.ToString())); 
	}
}
void AMagicOrbit::DestroyRamdomOrbit()
{
	if (SpawnedOrbits.Num() == 0)
		return;

	int32 Index = FMath::RandRange(0, SpawnedOrbits.Num() - 1);

	AMagicOrbit* OrbitToDestroy = SpawnedOrbits[Index];

	if (OrbitToDestroy)
	{
		ShowDebugMessage(FString::Printf(TEXT("The magic orbit [%d] was destroyed"), Index));

		OrbitToDestroy->Destroy();
	}
	SpawnedOrbits.RemoveAt(Index); 
}
void AMagicOrbit::ShowDebugMessage(const FString& Message) {
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Red, Message); 
		
	}
}