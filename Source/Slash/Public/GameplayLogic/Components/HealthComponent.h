// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UENUM(BlueprintType)
enum class EHealthState : uint8
{
	Alive,
	Dead
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	* @brief Subtrat damage from current health and calculate a clamped between 0 and MaxHealth.
	* 
	*/
	void ReceiveDamage(float Damage);

	void Heal(float HealAmount);

	/** @return Normalized Health Percent Calculation */
	float GetNormalizedHealthPercent();

	/** Update player health states to check if its alive */
	void UpdateHealthState();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	EHealthState HealthState;

private:

	/** Current health value */
	UPROPERTY(EditAnywhere, Category = "Health")
	float CurrentHealth = 100;

	/** Max health value */
	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100;

public:
	FORCEINLINE const EHealthState GetHealthState() { return HealthState; };

	FORCEINLINE void SetHealthState(EHealthState InHealthState) { HealthState = InHealthState; };

};
