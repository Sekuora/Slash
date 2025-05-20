// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatMontages.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH_API UCombatMontages : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatMontages();

private:
	UPROPERTY(EditAnywhere, Category = "Combat Montages")
	const TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditAnywhere, Category = "Combat Montages")
	const TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, Category = "Combat Montages")
	const TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Combat Montages")
	const TObjectPtr<UAnimMontage> EquipWeaponMontage;

	UPROPERTY(EditAnywhere, Category = "Combat Montages")
	const TObjectPtr<UAnimMontage> FaceToTargetMontage;

	UPROPERTY(EditAnywhere, Category = "Combat Montages")
	const TObjectPtr<UAnimMontage> DodgeMontage;



public:	
	

	FORCEINLINE const TObjectPtr<UAnimMontage> GetDeathMontage(){ return DeathMontage; };
	FORCEINLINE const TObjectPtr<UAnimMontage> GetHitReactMontage() { return HitReactMontage; };
	FORCEINLINE const TObjectPtr<UAnimMontage> GetAttackMontage() { return AttackMontage; };
	FORCEINLINE const TObjectPtr<UAnimMontage> GetEquipWeaponMontage() { return EquipWeaponMontage; };
	FORCEINLINE const TObjectPtr<UAnimMontage> GetFaceToTargetMontage() { return FaceToTargetMontage; };

	FORCEINLINE const TObjectPtr<UAnimMontage> GetDodgeMontage() { return DodgeMontage; };
};
