// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatStates.h"
#include "CombatComponent.generated.h"


class UMontagePlayerComponent;
class UCombatMontages;
class UHealthBarWidgetComponent;
class UHitNexus;
class UWeaponsCore;


/**
*	@brief High level - behavior driver for combat actions
*	
*	
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:



	// Sets default values for this component's properties
	UCombatComponent();
	
	//
	// Death Behavior
	//

	/** Check if an entity is death and call deafult behavior if delegate is not bound */
	UFUNCTION()
	void WhenEntityDies();

	/** Default death behavior */
	UFUNCTION()
	void OnDeath();

	///** Custom Death Delegate */
	//FCustomDeathDelegate OnCustomDeath;

	//
	// Attack Behavior
	//

	/** Repeatable function to play attack montage */
	UFUNCTION()
	void PlayAttackMontage();

	/** Starts attack timer on demand */
	void StartAttackTimer();

	/** Stops attack timer on demand */
	void StopAttackTimer();

	//
	// Hit Behavior
	//


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Action State Enum
	ECombatState CombatState;

private:



	UPROPERTY(VisibleInstanceOnly, Category = "Owner Character")
	TObjectPtr<ACharacter> Character;

	//
	// Linked Components
	//

	UPROPERTY(VisibleDefaultsOnly, Category = "Combat | Animations")
	TObjectPtr<UMontagePlayerComponent> MontagePlayer;

	UPROPERTY(VisibleDefaultsOnly, Category = "Combat | Animations")
	TObjectPtr<UCombatMontages> CombatMontages;

	UPROPERTY(EditInstanceOnly, Category = "Combat | Life Span")
	float DeadLifeSpan = 5.f;

	UPROPERTY(VisibleAnywhere, Category = "HUD")
	TObjectPtr<UHealthBarWidgetComponent> HealthBarWidget;

	UPROPERTY(VisibleAnywhere, Category = "Weapons Core")
	TObjectPtr<UWeaponsCore> WeaponsCore;

	UPROPERTY(VisibleAnywhere, Category = "Hit Nexus")
	TObjectPtr<UHitNexus> HitNexus;


	// Attack trigger timer
	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = "Combat | Attack")
	float MinAttackTime = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat | Attack")
	float MaxAttackTime = 0.9f;

	/** Define what happens on AttackEnd */
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void EmptyIgnoreActors();


public:	

	FORCEINLINE void SetOwnerCharacter(TObjectPtr<ACharacter> inOwnerCharacter ) { Character = inOwnerCharacter; };

	FORCEINLINE ECombatState GetCombatState() { return CombatState; };

	FORCEINLINE void SetCombatState(ECombatState InCombatState) { CombatState = InCombatState; };
};
