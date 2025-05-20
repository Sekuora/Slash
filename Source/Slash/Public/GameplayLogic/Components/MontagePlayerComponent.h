// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MontagePlayerComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SLASH_API UMontagePlayerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMontagePlayerComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//
	// Montages
	//

	/**
	* Collects FName section names array from a given montage.
	*
	* Usually the first step to play animation montages or randomize them.
	* 
	* @param AnimMontage - The montage from which section names must be collected.
	* @return - Array of Section Names
	*/
	TArray<FName> CollectMontageSectionNames(TObjectPtr<UAnimMontage> AnimMontage);


	/**
	* Plays animation section from a UAnimMontage
	*
	* @param AnimMontage - UAnimMontage to play.
	* @param SectionName - FName of the section to be played.
	*/
	void PlayMontageSection(TObjectPtr<UAnimMontage> AnimMontage, const FName& SectionName);


	/**
	* Randomizes an array of montage section names to play.
	*
	* Additionally it assigns the randomized montage as the current montage section to play.
	* 
	* @param SectionNames - TArray containing FName section names to randomize.
	*/
	void RandomizeMontageSection(TArray<FName> SectionNames);

	/**
	* Set BlendOut values of a montage.
	* 
	* Set bAutoBlendOut to false to disable sequences from going back to other animations after they end. 
	* 
	* @param AnimMontage - Montage to set blend out values.
	* @param bAutoBlendOut - Define if auto blend out is enabled.
	* @param TriggerTime - Trigger time of the blend out.
	*/
	void SetMontageBlendOut(TObjectPtr<UAnimMontage> AnimMontage, bool bAutoBlendOut, float TriggerTime);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;


private:
	TObjectPtr<USkeletalMeshComponent> MeshComponent;

	/** Stores a montage section name used as the current montage section to play. */
	UPROPERTY(VisibleAnywhere, Category = "Montages")
	FName CurrentMontageSection;

	UPROPERTY(VisibleAnywhere, Category = "Montages")
	TObjectPtr<UAnimInstance> AnimInstance;
	

public:
	FORCEINLINE void SetMeshComponent(TObjectPtr<USkeletalMeshComponent> OwnerMeshComponent)
	{
		MeshComponent = OwnerMeshComponent;
	}

	FORCEINLINE FName GetCurrentMontageSection() { return CurrentMontageSection; }

	FORCEINLINE TObjectPtr<UAnimInstance> GetAnimInstance() { return  AnimInstance; }

};

