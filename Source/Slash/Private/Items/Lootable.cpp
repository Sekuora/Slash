// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Lootable.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerDataSubsystem.h"



ALootable::ALootable() 
	: AItem()
{
	
}

void ALootable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// Add Rotation to Lootable
	FRotator DeltaRotation(DeltaTime * Pitch, DeltaTime * Yaw, DeltaTime * Roll);

	if (RootComponent)
	{
		RootComponent->AddWorldRotation(DeltaRotation, false, 0, ETeleportType::None);
	}
}

void ALootable::OnSphereOverlap(UPrimitiveComponent* OnComponentBeginOverlap, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UClass* OtherActorClass = OtherActor->GetClass();

	if (OtherActor && OtherActor->ActorHasTag("SlashCharacter"))
	{
		PlayerDataSubsystem->Broadcast_OnCollectedItem(Value, ItemID);

		if (PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
		}
	}

	Destroy();
}
