// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayLogic/Components/MontagePlayerComponent.h"
#include "HUD/HealthBarWidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Combat/CombatMontages.h"
#include "Combat/HitNexus.h"
#include "Combat/WeaponsCore.h"
#include "Kismet/GameplayStatics.h"
#include "Items/Weapons/Weapon.h"




// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACharacter>(GetOwner());

	// Drive behavior of entity direct components.

	if (Character)
	{
		MontagePlayer	= Character->FindComponentByClass<UMontagePlayerComponent>();
		CombatMontages	= Character->FindComponentByClass<UCombatMontages>();
		HealthBarWidget = Character->FindComponentByClass<UHealthBarWidgetComponent>();
		WeaponsCore		= Character->FindComponentByClass<UWeaponsCore>();
		HitNexus		= Character->FindComponentByClass<UHitNexus>();
	}

	// Pass sub objects to entity - emulated inheritance.


	/*	Tip:
	*	Call Combat State Ready on Blueprint AttackEnd anim notify to retrigger attack. 
	*/
}



void UCombatComponent::AttackEnd()
{
	// Set state back to ready
	CombatState = ECombatState::Ready;
	UE_LOG(LogTemp, Display, TEXT("Combat State: Ready"));
}

void UCombatComponent::EmptyIgnoreActors()
{
	// Flush Ignore Actors Array
	WeaponsCore->GetActiveWeapon()->GetIgnoreActors().Empty();

	UE_LOG(LogTemp, Display, TEXT("Weapon IgnoreActors Emptied"));
}


void UCombatComponent::StartAttackTimer()
{
	if (CombatState != ECombatState::Attacking)
	{
		CombatState = ECombatState::Attacking;
		UE_LOG(LogTemp, Display, TEXT("Combat State: Attacking"));
	}

	UE_LOG(LogTemp, Display, TEXT("AttackTimer Started"));
	const float TimerRandomRange = FMath::RandRange(MinAttackTime, MaxAttackTime);


	while (Character->GetWorldTimerManager().GetTimerRemaining(AttackTimer) <= 0)
	{
		Character->GetWorldTimerManager().SetTimer(AttackTimer, this, &UCombatComponent::PlayAttackMontage, TimerRandomRange);
	}
}

void UCombatComponent::StopAttackTimer()
{
	Character->GetWorldTimerManager().ClearTimer(AttackTimer);
}

void UCombatComponent::PlayAttackMontage()
{
	UE_LOG(LogTemp, Display, TEXT("Playing Attack Montage Started"));


	if (!MontagePlayer || !CombatMontages)
	{
		return;
	}

	// Play random attack animation

	MontagePlayer->RandomizeMontageSection(MontagePlayer->CollectMontageSectionNames(CombatMontages->GetAttackMontage()));

	MontagePlayer->PlayMontageSection(CombatMontages->GetAttackMontage(), MontagePlayer->GetCurrentMontageSection());
}


//
// Health state functions
//

void UCombatComponent::WhenEntityDies()
{
	OnDeath();

}

void UCombatComponent::OnDeath()
{
	// Play Dead Montage
	if (MontagePlayer && CombatMontages)
	{
		MontagePlayer->RandomizeMontageSection(MontagePlayer->CollectMontageSectionNames(CombatMontages->GetDeathMontage()));

		MontagePlayer->PlayMontageSection(CombatMontages->GetDeathMontage(), MontagePlayer->GetCurrentMontageSection());
	}

	// Set Values

	if (Character)
	{

		Character->GetCharacterMovement()->bOrientRotationToMovement = false;

		Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		Character->GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

		Character->GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

		Character->SetLifeSpan(DeadLifeSpan);
	}

	// If Entity has WeaponsCore set lifespan time to destroy;

	if (WeaponsCore)
	{
		WeaponsCore->DestroyActiveWeapon(DeadLifeSpan);
	}
	

	// Disable UI
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}

	StopAttackTimer();

	WeaponsCore->ToggleActiveWeaponCollision(ECollisionEnabled::NoCollision);
}
