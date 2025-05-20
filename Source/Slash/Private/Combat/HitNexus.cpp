// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/HitNexus.h"
#include "Kismet/GameplayStatics.h"

void UHitNexus::BasicHit(FVector ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetOwner()->GetWorld(), HitSound, ImpactPoint);
	}

	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetOwner()->GetWorld(), HitParticles, ImpactPoint);
	}
}
