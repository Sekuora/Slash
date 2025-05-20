// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HitNexus.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API UHitNexus : public UActorComponent
{
	GENERATED_BODY()


public:
	void BasicHit(FVector ImpactPoint);


private:

	UPROPERTY(EditAnywhere, Category = "Hit Sound Effects")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Hit Visual Effects")
	UParticleSystem* HitParticles;



public:

	// Impact Point


	// Hit Sound Effects

	FORCEINLINE USoundBase* GetHitSound() { return HitSound; };

	// Hit Particles

	FORCEINLINE UParticleSystem* GetHitParticles() { return HitParticles; };

};

