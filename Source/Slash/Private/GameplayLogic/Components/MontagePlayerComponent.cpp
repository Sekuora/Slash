// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayLogic/Components/MontagePlayerComponent.h"

// Sets default values for this component's properties
UMontagePlayerComponent::UMontagePlayerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UMontagePlayerComponent::BeginPlay()
{
	Super::BeginPlay();

	// Set Mesh Component on actor using this component.

	if (!MeshComponent)
	{
		return;
	}

	AnimInstance = MeshComponent->GetAnimInstance();
}


// Called every frame
void UMontagePlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

// Montage Playing Functionality


TArray<FName> UMontagePlayerComponent::CollectMontageSectionNames(TObjectPtr<UAnimMontage> AnimMontage)
{
	TArray<FName> SectionNames;


	if (AnimMontage)
	{
		for (const FCompositeSection& Section : AnimMontage->CompositeSections)
		{
			SectionNames.AddUnique(Section.SectionName);

			UE_LOG(LogTemp, Display, TEXT("Obtained FNames: %s"), *Section.SectionName.ToString());
		}
	}

	return SectionNames;
}

void UMontagePlayerComponent::PlayMontageSection(TObjectPtr<UAnimMontage> AnimMontage, const FName& SectionName)
{

	if (AnimInstance && AnimMontage)
	{
		AnimInstance->Montage_Play(AnimMontage);
		AnimInstance->Montage_JumpToSection(SectionName, AnimMontage);
	}
}


void UMontagePlayerComponent::RandomizeMontageSection(TArray<FName> SectionNames)
{

	if (SectionNames.Num() <= 0)
	{
		return;
	}
	
	// Set Random Number for selection
	const int32 RandomSelection = FMath::RandRange(0, SectionNames.Num() - 1);

	CurrentMontageSection = SectionNames[RandomSelection];

	UE_LOG(LogTemp, Display, TEXT("Current Montage Section: %s"), *CurrentMontageSection.ToString());
}

void UMontagePlayerComponent::SetMontageBlendOut(TObjectPtr<UAnimMontage> AnimMontage, bool bAutoBlendOut, float TriggerTime)
{
	if (!AnimMontage)
	{
		return;
	}

	AnimMontage->bEnableAutoBlendOut = bAutoBlendOut;
	AnimMontage->BlendOutTriggerTime = TriggerTime;
}








