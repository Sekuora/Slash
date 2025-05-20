// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitHandler.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHitHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * Handles what happens when an object gets hit by players
 */
class SLASH_API IHitHandler
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/**
	* Define what happens when a hit occurs.
	* It is a BlueprintNativeEvent, making it overwritable in cpp and and bluerints.
	*/
	UFUNCTION(BlueprintNativeEvent)
	void GetHit(const FVector& ImpactPoint);
};


