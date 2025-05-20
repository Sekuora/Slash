// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"



class UBoxComponent;
class ASlashFieldSystem;

/**
 * Weapon
 */
UCLASS()
class SLASH_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();

	UFUNCTION()
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);

	void TieToSocket(USceneComponent* InParent, const FName& InSocketName);

protected:
	virtual void BeginPlay() override;


	virtual void OnSphereOverlap(
		UPrimitiveComponent* OnComponentBeginOverlap, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	/**		@brief HitBox Overlap Event for Weapons */
	UFUNCTION()
	void OnHitBoxOverlap(UPrimitiveComponent* OnComponentBeginOverlap, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);


	/**		@brief Spawns a field system to affect chaos objects. */
	UFUNCTION()
	void SpawnFieldSystem(const FVector& FieldLocation);

	/**		@brief Field system */
	UPROPERTY(EditAnywhere, Category = "Components")
	TSubclassOf<ASlashFieldSystem> FieldSystem;

	//
	// Properties
	//

	/** Damage Value */
	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float Damage = 20.f;


private:

	//
	// Weapon components
	//

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> HitBox;

	/** Hit Ray Start */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> HitRayStart;

	/** Hit Ray End */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> HitRayEnd;

	/** List of actors to ignore */
	TArray<AActor*> IgnoreActors;

	//
	// SFX
	//

	/** Equip Sound */
	UPROPERTY(EditAnywhere, Category = "Sound Effects")
	TObjectPtr<USoundBase> EquipSound;



	// Getters / Setters
public:
	// Hit Box
	FORCEINLINE TObjectPtr<UBoxComponent> GetHitBox() const { return HitBox; }

	// Ignore Actors Array
	FORCEINLINE TArray<AActor*>& GetIgnoreActors() { return IgnoreActors; };

	// Sounds
	FORCEINLINE TObjectPtr<USoundBase> GetEquipSound() { return EquipSound; };
	
	// VFX
	FORCEINLINE TObjectPtr<UNiagaraComponent> GetItemEffect() { return ItemEffect; };

	// Sphere
	FORCEINLINE TObjectPtr<USphereComponent> GetSphere() { return Sphere; };

	// Item State
	FORCEINLINE void SetItemState(EItemState inItemState) { ItemState = inItemState; };

	// Item Mesh
	FORCEINLINE TObjectPtr<UStaticMeshComponent> GetItemMesh() { return ItemMesh; };
};
