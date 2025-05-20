// Fill out your copyright notice in the Description page of Project Settings.



#include "Characters/SlashAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Characters/SlashCharacter.h"

void USlashAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ASlashCharacter>(TryGetPawnOwner());
	if (Character)
	{
		CharacterMovementComponent = Character->GetCharacterMovement();
	}
}

void USlashAnimInstance::NativeUpdateAnimation(float DeltaTime)
{

	Super::NativeUpdateAnimation(DeltaTime);
	if (CharacterMovementComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(CharacterMovementComponent->Velocity);

		IsFalling = CharacterMovementComponent->IsFalling();

		// Set the CharacterState in AnimInstance to the gathered CharacterState of the SlashCharacter
		CharacterState = Character->GetCharacterState();
	}

}
