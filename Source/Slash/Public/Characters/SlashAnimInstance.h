// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterStates.h"
#include "SlashAnimInstance.generated.h"

UCLASS()
class SLASH_API USlashAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

	// Get Character Class
	UPROPERTY(BlueprintReadOnly)
	class ASlashCharacter* Character;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	class UCharacterMovementComponent* CharacterMovementComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool IsFalling;

	// | defines a subcategory for UPROPERTY
	UPROPERTY(BlueprintReadOnly, Category = "Character State")
	ECharacterState CharacterState;
};
