// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapSpawner.h"
#include "FlyingTrap.h"


// Sets default values
ATrapSpawner::ATrapSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}
bool ATrapSpawner::SpawnFlyingTrapAtLocation(FVector Location)
{
	UWorld* World = GetWorld(); 
	// Spawn 1 FlyingTrap at location 
	AFlyingTrap* NewTrap = World->SpawnActor<AFlyingTrap>(AFlyingTrap::StaticClass(), Location, FRotator::ZeroRotator); 
	
	// if spawn successfully 
	if (NewTrap)
	{
		SpawnedTraps.Add(NewTrap); 

		// Debug Message 
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(10, 2.f, FColor::Blue, FString::Printf(TEXT("Spawned trap: %s"), *NewTrap->GetName())); 
		}
		return true; 
	}
	return false; 

}
bool ATrapSpawner::DestroyTrapByName(const FString& TrapName)
{
	for (int32 i = 0; i < SpawnedTraps.Num(); i++) {
		AFlyingTrap* Trap = SpawnedTraps[i]; 

		if (Trap && Trap->GetName().Contains(TrapName))
		{
			Trap->Destroy(); 
			SpawnedTraps.RemoveAt(i); 

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(11, 2.f, FColor::Red, FString::Printf(TEXT("Destroyed trap: %s"), *TrapName));
			}
			return true;
		}
	}
	return false; 
}
// Called when the game starts or when spawned
void ATrapSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrapSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

