// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Lootable.generated.h"


/**
 * 
 */
UCLASS()
class SLASH_API ALootable : public AItem
{
	GENERATED_BODY()

public:
	ALootable();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OnComponentBeginOverlap, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult) override;


private:
	// SFX
	UPROPERTY(EditAnywhere, Category = "Sound Effects")
	USoundBase* PickupSound;


	//
	// Propreties
	//

	UPROPERTY(EditAnywhere, Category = "Loot Properties")
	int32 Value = 0;

	UPROPERTY(EditAnywhere, Category = "Mesh Properties")
	float Yaw = 0.f;

	UPROPERTY(EditAnywhere, Category = "Mesh Properties")
	float Pitch = 0.f;

	UPROPERTY(EditAnywhere, Category = "Mesh Properties")
	float Roll = 0.f;

};