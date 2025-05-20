// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterStates.h"
#include "Interfaces/HitHandler.h"
#include "SlashCharacter.generated.h"


// Forward Declarations
class UInputMappingContext;
class UInputAction;
class UAnimMontage;
class UCombatMontages;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class UMontagePlayerComponent;
class UDirectionalHitReactComponent;
class UHealthComponent;
class UWeaponsCore;
class UHitNexus;
class AItem;
class AWeapon;
class APlayerHUD;
class UPlayerOverlayUserWidget;
class UPlayerDataSubsystem;


UCLASS()
class SLASH_API ASlashCharacter : public ACharacter, public IHitHandler
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;

	UFUNCTION(BlueprintCallable)
	void ToggleWeaponCollision(ECollisionEnabled::Type CollisionEnabled);
	
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);


protected:

	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data");
	int32 CurrentGoldAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data");
	int32 CurrentSoulsAmount;

	// Input Mapping Context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
	TObjectPtr<UInputMappingContext> InputMapping;

	// Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
	TObjectPtr<UInputAction> EquipAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
	TObjectPtr<UInputAction> DodgeAction;

	UPROPERTY(BlueprintReadWrite)
	EActionState ActionState = EActionState::Ready;

	UPROPERTY(BlueprintReadWrite)
	ETrackingState TrackingState;


	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Tag")
	FName SlashCharacterTag = "SlashCharacter";

	// InputAction Functions

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EquipWeapon(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	void Dodge(const FInputActionValue& Value);

	// Montage Functions

	bool CanAttack();

	bool CanDisarm();

	bool CanArm();

	UFUNCTION(BlueprintCallable)
	void TrackingStateReady();

private:
	
	UPlayerDataSubsystem* PlayerDataSubsystem;

	
	// Enum States
	ECharacterState CharacterState = ECharacterState::Unarmed;

	//
	// Components
	//


	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, Category = "Hair Components")
	TObjectPtr<UGroomComponent> Hair;

	UPROPERTY(VisibleAnywhere, Category = "Hair Components")
	TObjectPtr<UGroomComponent> Eyebrows;

	UPROPERTY(VisibleInstanceOnly, Category = "Items");
	TObjectPtr<AItem> OverlappingItem;



	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<AWeapon> Weapon;

	// HUD
	UPROPERTY(VisibleAnywhere, Category = "HUD")
	TObjectPtr<APlayerHUD> PlayerHUD;

	UPROPERTY(VisibleAnywhere, Category = "HUD")
	UPlayerOverlayUserWidget* PlayerWidget;

	// Combat Animation Montages

	UPROPERTY(EditDefaultsOnly, Category = "Combat Montages")
	TObjectPtr<UCombatMontages> CombatMontages;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMontagePlayerComponent> MontagePlayer;

	//
	// Hit Behavior
	// 

	bool bIsMoving;

	UPROPERTY(EditAnywhere, Category = "Hit Nexus");
	TObjectPtr<UHitNexus> HitNexus;

	UPROPERTY(EditAnywhere, Category = "Hit React");
	TObjectPtr<UDirectionalHitReactComponent> HitReact;

	//
	// Blueprint Callable Functions
	//

	/** Define what happens on Disarm - As in disarmed a weapon */
	UFUNCTION(BlueprintCallable)
	void Disarm();


	/** Define what happens on Arm - As in armed a weapon */
	UFUNCTION(BlueprintCallable)
	void Arm();

	/** Define what happens on GearEnd */
	UFUNCTION(BlueprintCallable)
	void onGearEnd();

	/** Define what happens on AttackEnd */
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	// Events Binding
	// 
	// OnCollectedItem Event Function Callback
	UFUNCTION(BlueprintCallable)
	void OnCollectedItemEvent(const int32& Amount, const FName& ID);


	// Setters and getters
public:

	

	// Set Item
	FORCEINLINE void SetOverlappingItem(TObjectPtr<AItem> Item) { OverlappingItem = Item; }

	// Get CharacterState
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
};
