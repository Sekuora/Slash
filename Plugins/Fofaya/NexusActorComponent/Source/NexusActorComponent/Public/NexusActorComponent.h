// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NexusActorComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NEXUSACTORCOMPONENT_API UNexusActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UNexusActorComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** Manage Details Panel Excluded Categories */
	static TArray<FName> DetailsPanelExcludedCategories;

private:



	/** Array of linked components */
	UPROPERTY(VisibleAnywhere, Category = "Linked Components Settings")
	TArray<TObjectPtr<UNexusActorComponent>> LinkedComponents;

	/** Instance required to get this class instance in other classes: Specially useful for NexusActorDetailsPanel */
	static UNexusActorComponent* Instance;

public:

	/** Getter for this actor instance */
	static UNexusActorComponent* GetInstace();

	/** Getter for array List of linked actor components */
	FORCEINLINE TArray<TObjectPtr<UNexusActorComponent>> GetLinkedComponents() { return LinkedComponents; };

	/** Getter for exluded names from detail panels list */
	FORCEINLINE TArray<FName> GetDetailsPanelExcludedCategories() { return DetailsPanelExcludedCategories; };
};
