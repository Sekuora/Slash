// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DirectionalHitReactComponent.generated.h"


UENUM(BlueprintType) // UENUM exposes enums to blueprint
enum class EDirectionalHitState : uint8
{
	HitFromBack,
	HitFromFront,
	HitFromRight,
	HitFromLeft,
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SLASH_API UDirectionalHitReactComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDirectionalHitReactComponent();

	/**
	 * @brief Calculates the direction and side of an impact.
	 *
	 * This function calculates the dot product to determine the angle theta and the direction
	 * from which an impact has occurred. It then calculates the cross product to determine
	 * if the impact direction is from the left or right.
	 *
	 * @param ImpactPoint - The FVector where an impact has occurred.
	 * @return Theta - The theta value in reference to the ImpactPoint.
	 */
	double FetchThetaFromHit(const FVector& ImpactPoint);


	/**
	* @brief Determines section to play from a HitReact animation montage.
	*
	* First: calls MapHitDirectionToState to handle sections depending on 
	* EDirectionalHitState enum.
	* 
	* Conditions the section to be played taking theta angle as parameter.
	* 
	* 
	* @param Theta - Theta angle to determine section to be played.
	* @return Section - FName of section to be played.
	*/
	FName SelectHitReactSection(const double& Theta);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;



private:

	/**
	* @brief Maps hit react conditions to enum EDirectionalHitState.
	*
	* @param Theta - Theta angle to determine section to be played.
	*/
	void MapHitDirectionToState(const double& Theta);

	TObjectPtr<AActor> OwnerActor;

	EDirectionalHitState DirectionalHitState;


public:
	FORCEINLINE void SetOwnerActor(TObjectPtr<AActor> InOwnerActor) { OwnerActor = InOwnerActor; };

	FORCEINLINE EDirectionalHitState GetDirectionalHitState() { return DirectionalHitState; };

};
