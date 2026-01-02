#include "CastlePlayerCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"


#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"

ACastlePlayerCharacter::ACastlePlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	Camera->SetupAttachment(CameraSpringArm);
	CameraSpringArm->SetupAttachment(RootComponent);

	bUseControllerRotationPitch = bUseControllerRotationRoll = false; 
	bUseControllerRotationYaw = true; 

	CameraSpringArm->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false; 
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f); 


	// Collision Setup
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("OverlapAll"));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACastlePlayerCharacter::OnPlayerBeginOverlap);
}

void ACastlePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LP = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsystem->AddMappingContext(GameplayIMC, 0);
			}
		}
	}
}

void ACastlePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Line Trace Forward
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * 1000.f;

	FHitResult HitResult;
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, this);
	TraceParams.bReturnPhysicalMaterial = false;

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams);

	if (bHit && HitResult.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit: %s"), *HitResult.GetActor()->GetName());
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.5f, 0, 2.0f);
	}
	
}

void ACastlePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(MoveIA, ETriggerEvent::Triggered, this, &ACastlePlayerCharacter::Move);
		EIC->BindAction(LookIA, ETriggerEvent::Triggered, this, &ACastlePlayerCharacter::Look);
		EIC->BindAction(JumpIA, ETriggerEvent::Started, this, &ACastlePlayerCharacter::JumpOrStopJumping);
		EIC->BindAction(JumpIA, ETriggerEvent::Completed, this, &ACastlePlayerCharacter::JumpOrStopJumping);
	}
}

void ACastlePlayerCharacter::Move(const FInputActionValue& Value)
{
	FRotator RotationVector = FRotator(0.f, GetControlRotation().Yaw, 0.f); 
	FMatrix RotationMatrix = FRotationMatrix::Make(RotationVector);

	FVector2D MoveVector = Value.Get<FVector2D>(); 
	AddMovementInput(RotationMatrix.GetUnitAxis(EAxis::X), MoveVector.Y); 
	AddMovementInput(RotationMatrix.GetUnitAxis(EAxis::Y), MoveVector.X); 

}

void ACastlePlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxis = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxis.X);
	AddControllerPitchInput(LookAxis.Y);
}

void ACastlePlayerCharacter::JumpOrStopJumping(const FInputActionValue& Value)
{
	bool bPressed = Value.Get<bool>();
	if (bPressed)
	{
		Jump();
	}
	else
	{
		StopJumping();
	}
}

void ACastlePlayerCharacter::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Overlapped with: %s"), *OtherActor->GetName()));
	}
}
