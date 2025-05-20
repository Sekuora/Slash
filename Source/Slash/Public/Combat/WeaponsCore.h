// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponsCore.generated.h"

class AWeapon;

/**
 *
 */
UCLASS()
class SLASH_API UWeaponsCore : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponsCore();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** @brief Destroy active weapon, useful for events that require its destruction. */
	void DestroyActiveWeapon(float ActiveWeaponLifeSpan);


	/** @brief Spawn a weapon and return its pointer reference. */
	TObjectPtr<AWeapon> SpawnWeapon(TSubclassOf<AWeapon>);

	/** @brief Function to equip an spawned AWeapon with a pointer reference. */
	void EquipWeapon(TObjectPtr<AWeapon> WeaponToEquip, USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);

	void AttachToSocket(TObjectPtr<AWeapon> WeaponToAttach, USceneComponent* InParent, const FName& InSocketName);

	void ToggleActiveWeaponCollision(ECollisionEnabled::Type CollisionEnabled);

	UFUNCTION(BlueprintCallable)
	void ToggleWeaponCollision(ECollisionEnabled::Type CollisionEnabled);

protected:

	// Begin Play
	virtual void BeginPlay() override;


private:

	//
	// Weapons Managment
	//

	/** 
	*	@brief Optional: Define a default weapon - in editor.
	* 
	*	Useful for entities that need to start the game with a weapon.
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Management")
	TSubclassOf<AWeapon> DefaultWeapon;


	/** @brief The current weapon that's being used for combat. */
	UPROPERTY(VisibleAnywhere, Category = "Weapon Management")
	TObjectPtr<AWeapon> ActiveWeapon;

public:
	/** Getter method for default weapon */
	FORCEINLINE TSubclassOf<AWeapon> GetDefaultWeapon() { return DefaultWeapon; };

	/** Set a weapon as the current Active Weapon */
	FORCEINLINE void SetActiveWeapon(TObjectPtr<AWeapon> InWeapon) { ActiveWeapon = InWeapon; };

	/** Get the current Active Weapon */
	FORCEINLINE TObjectPtr<AWeapon> GetActiveWeapon() { return ActiveWeapon; };
};
