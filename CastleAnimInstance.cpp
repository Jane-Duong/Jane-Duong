// Fill out your copyright notice in the Description page of Project Settings.


#include "CastleAnimInstance.h"
#include "CastlePlayerCharacter.h" 
#include "GameFramework/CharacterMovementComponent.h"

void UCastleAnimInstance::NativeInitializeAnimation() 
{ 
	Super::NativeInitializeAnimation(); 
	OwningCharacter = Cast<ACastlePlayerCharacter>(TryGetPawnOwner()); 
	if (OwningCharacter) 
	{ 
		CharacterMovementComp = OwningCharacter->GetCharacterMovement(); 
	} 
}
void UCastleAnimInstance::NativeUpdateAnimation(float DeltaTime) {
	Super::NativeUpdateAnimation(DeltaTime); 
	if (CharacterMovementComp) 
	{
		GroundSpeed = CharacterMovementComp->Velocity.Size2D(); 
		// update speed and falling 
		bIsFalling = CharacterMovementComp->IsFalling(); 
	}
	if (bJustLanded) 
	{ 
		bJustLanded = false; 
	} 
	if (CharacterMovementComp) 
	{ //Calculate movement direction 
		FVector CharacterGroundVelocity = CharacterMovementComp->Velocity; 
		CharacterGroundVelocity.Z = 0.f; 
		FRotator CharacterGroundRotation = OwningCharacter->GetActorRotation();
		CharacterGroundRotation.Roll = CharacterGroundRotation.Pitch = 0.f; 
		CharacterDirection = CalculateDirection(CharacterGroundVelocity, CharacterGroundRotation);
	}
}