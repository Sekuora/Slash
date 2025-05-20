// Copyright 2024 Fofaya Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitHandler.h"
#include "EnemyStates.h"
#include "Enemy.generated.h"

class UAnimMontage;
class USoundBase;
class UHealthComponent;
class UHealthBarWidgetComponent;
class AAIController;
class UPawnSensingComponent;
class UMontagePlayerComponent;
class UDirectionalHitReactComponent;
class UCombatMontages;
class UCombatComponent;
class UWeaponsCore;
class UHitNexus;
class ALootable;

UCLASS()
class SLASH_API AEnemy : public ACharacter, public IHitHandler
{
	GENERATED_BODY()

public:

	AEnemy();


	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	* @brief Handles the logic for when actor gets hit.
	*
	* This implemetation gets a theta angle to determine where the hit comes from.
	* Subsequently, plays a different montage section depending on hit angle.
	*
	* @param ImpactPoint - The FVector where an impact has occurred.
	*/
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:

	virtual void BeginPlay() override;

	//
	// Health
	//

	/** HealthComponent Component to manage health related operations */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components | Health")
	TObjectPtr<UHealthComponent> HealthComponent;

	//
	// AI Navigation
	//

	/** Min Patrol Idle Time for random patrol idle timer  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Navigation | Patrol")
	float MinPatrolIdleTime = 3.f;

	/** Max Patrol Idle Time for random patrol idle timer  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Navigation | Patrol")
	float MaxPatrolIdleTime = 15.f;

	/** Actions executed while in the range of the vision PawnSensingComponent */
	UFUNCTION()
	void WhenSeePawn(APawn* SeenPawn);

	/** Radius where a differet pawn can be seen */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Navigation | Chase Pawn")
	float PawnSightRadius = 1500.f;

	/** Radius where combat events can occurr */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Navigation | Combat")
	double CombatRadius = 1000.f;

	/** Attack Radius for close combat */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Navigation | Combat")
	double AttackRadius = 150.f;

	// AI Components / Values

	/** Define the radius of a patrol target. On entering radius can go to next patrol */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Component | Walk Speed Values")
	float PatrolWalkSpeed = 200.f;

	/** Define the radius of a patrol target. On entering radius can go to next patrol */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Component | Walk Speed Values")
	float ChaseWalkSpeed = 400.f;

	/** Define the radius of a patrol target. On entering radius can go to next patrol */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Component | Walk Speed Values")
	float CombatWalkSpeed = 500.f;

	// Loot Items
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Data")
	TSubclassOf<ALootable> SoulLoot;

	//
	// Combat Component
	// 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCombatComponent> CombatComponent;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UHitNexus> HitNexus;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UWeaponsCore> WeaponsCore;

	UPROPERTY(BlueprintReadOnly)
	ETargetTransitiveState TargetTransitiveState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	double LookAtTargetPitch;

	//
	// Entity States
	//

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ENavigationStates NavigationState = ENavigationStates::PatrolMode;


private:
	
	//
	//	Combat
	//

	const FName FactionTag = "Faction.Enemy"; 

	/** Handle what occurs when a combat target is assigned */
	void UpdateCombatTargetRadius();

	/** Actor gathered as combat target */
	UPROPERTY()
	AActor*CombatTarget;

	/** Check wether an Actor to target is in targetable radius */
	bool InTargetRange(AActor* ActorToTarget, double Radius);

	/** Handle target movement for AI controller */
	void MoveToTarget(TObjectPtr<AActor> PatrolTarget);

	/** Choose a random Next Target from PatrolTargets Array */
	void RandomizeNextPatrolTarget();

	/** Speed at which entity rotates to face a target */
	UPROPERTY(EditAnywhere);
	float LookAtTargetSpeed = 0.5f;


	//
	// Entity States
	//

	ETargetStates TargetState;

	ETransitiveStates TransitiveState;



	//
	// Combat
	//

	/** Directional Hit React Component */
	TObjectPtr<UDirectionalHitReactComponent> DirectionalHitReact;




	//
	// AI Navigation
	//

	/** AI Controller to perform actor actions */
	UPROPERTY()
	TObjectPtr<AAIController> AIController;

	/** Initial patrol target to moves towards */
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TObjectPtr<AActor> InitialPatrolTarget;

	/** Current patrol target to moves towards */
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TObjectPtr<AActor> CurrentPatrolTarget;

	/** Previous patrol target stored */
	UPROPERTY(VisibleInstanceOnly, Category = "AI Navigation")
	TObjectPtr<AActor> PreviousPatrolTarget;


	/** Array of patrol targets */
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<TObjectPtr<AActor>> PatrolTargets;

	/** Define the radius of a patrol target. On entering radius can go to next patrol */
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double PatrolTargetRadius = 200.f;

	/** Time to wait before going to next patrol target */
	FTimerHandle PatrolIdleTimer;

	/** What to call on patrol idle timer completion */
	void PatrolIdleTimerComplete();

	/** In charge of finding new patrol targets */
	void FindPatrolTarget();

	/** Pawn Sensing Component */
	UPROPERTY(VisibleAnywhere, Category = "AI Navigation | Pawn Sensing Component")
	UPawnSensingComponent* PawnSensing;

	//
	// Health
	//

	/** Time to wait before destroying  */
	UPROPERTY(EditAnywhere, Category = "Enemy | Components")
	float LifeSpan;

	//
	// Health / HUD
	//

	/** HUD HealthComponent Bar */
	UPROPERTY(VisibleAnywhere, Category = "Components | HUD")
	TObjectPtr<UHealthBarWidgetComponent> HealthBarWidget;
	 
	//
	// Montages
	//

	/** Montage Player Component */
	UPROPERTY()
	TObjectPtr<UMontagePlayerComponent> MontagePlayer;


	UPROPERTY(EditDefaultsOnly, Category = "Combat Montages")
	TObjectPtr<UCombatMontages> CombatMontages;

	//
	// SFX
	//

	/** Assign Hit Sound Effect */
	UPROPERTY(EditAnywhere, Category = "Sound Effects")
	USoundBase* HitSound;

	// VFX
	UPROPERTY(EditAnywhere, Category = "Visual Effects")
	UParticleSystem* HitParticles;

public:

	FORCEINLINE ETransitiveStates GetTransitiveState() { return TransitiveState; };

	FORCEINLINE const FName GetFactionTag() { return FactionTag; };
};
