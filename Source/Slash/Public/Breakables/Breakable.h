// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitHandler.h"
#include "BreakableStates.h"
#include "Breakable.generated.h"


class USoundBase;
class UCapsuleComponent;
class ALootable;





UCLASS()
class SLASH_API ABreakable : public AActor, public IHitHandler
{
	GENERATED_BODY()
	
public:	

	ABreakable();

	virtual void Tick(float DeltaTime) override;


	virtual void GetHit_Implementation(const FVector& ImpactPoint);


protected:

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UCapsuleComponent> CapsuleVolume;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DestroyedLifeSpan;


private:

	// Breakable Geometry
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGeometryCollectionComponent> GeometryCollection;

	//
	// SFX
	//
	
	/** Sound to play when breakable gets hit */
	UPROPERTY(EditAnywhere, Category = "Sound Effects")
	USoundBase* HitSound;

	//
	// Loot
	//

	/** Items Array to spawn as loot */
	UPROPERTY(EditDefaultsOnly, Category = "Loot Properties")
	TArray<TSubclassOf<ALootable>> LootItems;


	/** The Z axis height used to spawn a loot item */
	UPROPERTY(EditAnywhere, Category = "Loot Properties", meta = (DisplayName = "Z Spawn Offset"))
	float ZSpawnOffset = 75.f;

	/** Chaos Break Event Handler */
	UFUNCTION()
	void HandleChaosBreak(const FChaosBreakEvent& BreakEvent);

	UPROPERTY(VisibleAnywhere, Category = "Breakable Properties")
	EBreakableState BreakableState;


public:	

	FORCEINLINE TObjectPtr<UCapsuleComponent> GetCapsuleVolume() const { return CapsuleVolume; }

};
