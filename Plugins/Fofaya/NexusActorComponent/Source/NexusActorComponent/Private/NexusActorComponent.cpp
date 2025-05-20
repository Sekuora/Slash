// Fill out your copyright notice in the Description page of Project Settings.


#include "NexusActorComponent.h"


UNexusActorComponent* UNexusActorComponent::Instance = nullptr;

TArray<FName> UNexusActorComponent::DetailsPanelExcludedCategories;


// Sets default values for this component's properties
UNexusActorComponent::UNexusActorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}


// Called when the game starts
void UNexusActorComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UNexusActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

UNexusActorComponent* UNexusActorComponent::GetInstace()
{
	if (!Instance)
	{
		Instance = NewObject<UNexusActorComponent>();
	}
	return Instance;
}



