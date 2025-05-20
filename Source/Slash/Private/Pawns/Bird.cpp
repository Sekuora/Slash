// Fill out your copyright notice in the Description page of Project Settings.

#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


// Sets default values
ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Root")); // Root Component
	Capsule->SetCapsuleHalfHeight(20.f);
	Capsule->SetCapsuleRadius(15.f);
	SetRootComponent(Capsule);

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh")); // SK_Mesh
	BirdMesh->SetupAttachment(RootComponent);

	MoveComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent")); // Floating Movement Component
	MoveComponent->UpdatedComponent = Capsule;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm")); // Spring Arm
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 150.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera")); // Camera
	Camera->SetupAttachment(SpringArm);


	AutoPossessPlayer = EAutoReceiveInput::Player0;
}


void ABird::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (BirdInputMapping)
			{
				InputSystem->AddMappingContext(BirdInputMapping, 0);
			}
		}
	}
}


void ABird::Move(const FInputActionValue& Value)
{
	// Ctrl + space brings up intellisense back for code suggestions


	const float DirectionValue = Value.Get<float>();
	UE_LOG(LogTemp, Warning, TEXT("Gathered Float Directional Value"));

	if (Controller && (DirectionValue != 0.f))
	{
		FVector MovementValue = GetActorForwardVector();
		AddMovementInput(MovementValue, DirectionValue);
	}
}

void ABird::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	
	if (Controller)
	{
		AddControllerYawInput(LookAxisValue.X);	// Add rotation about X axis from mouse input
		AddControllerPitchInput(LookAxisValue.Y); // Add rotation about Y axis from mouse input
	}


}

// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::Move);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABird::Look);

	}
}

