// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayLogic/Components/HealthComponent.h"

UHealthComponent::UHealthComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::ReceiveDamage(float Damage)
{

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);

}

void UHealthComponent::Heal(float HealAmount)
{

	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.f, MaxHealth);

}

float UHealthComponent::GetNormalizedHealthPercent()
{
	return CurrentHealth / MaxHealth;
}

void UHealthComponent::UpdateHealthState()
{
	if (CurrentHealth > 0.f)
	{
		HealthState = EHealthState::Alive;
	}

	else
	{
		HealthState = EHealthState::Dead;
	}
}



