// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayLogic/Components/DirectionalHitReactComponent.h"

// Sets default values for this component's properties
UDirectionalHitReactComponent::UDirectionalHitReactComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UDirectionalHitReactComponent::BeginPlay()
{
	Super::BeginPlay();
}


double UDirectionalHitReactComponent::FetchThetaFromHit(const FVector& ImpactPoint)
{

	const FVector Forward = OwnerActor->GetActorForwardVector();

	const FVector FlatImpactPoint(ImpactPoint.X, ImpactPoint.Y, OwnerActor->GetActorLocation().Z);

	const FVector ToHit = (FlatImpactPoint - OwnerActor->GetActorLocation()).GetSafeNormal();

	const double& CosTheta = FVector::DotProduct(Forward, ToHit);

	double Theta = FMath::Acos(CosTheta);

	Theta = FMath::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);

	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	return Theta;
}

void UDirectionalHitReactComponent::MapHitDirectionToState(const double& Theta)
{
	if (!Theta)
	{
		return;
	}

	bool bIsBackHit = Theta >= 135.f && Theta <= 180.f;
	bool bIsFrontHit = Theta >= -45.f && Theta < 45.f;
	bool bIsLeftHit = Theta >= -135.f && Theta < -45.f;
	bool bIsRightHit = Theta >= 45.f && Theta < 135.f;

	if (bIsBackHit)
	{
		DirectionalHitState = EDirectionalHitState::HitFromBack;
	}

	else if (bIsFrontHit)
	{
		DirectionalHitState = EDirectionalHitState::HitFromFront;
	}

	else if (bIsRightHit)
	{
		DirectionalHitState = EDirectionalHitState::HitFromRight;
	}

	else if (bIsLeftHit)
	{
		DirectionalHitState = EDirectionalHitState::HitFromLeft;
	}
}

FName UDirectionalHitReactComponent::SelectHitReactSection(const double& Theta)
{
	MapHitDirectionToState(Theta);

	FName Section;

	switch (DirectionalHitState)
	{
	case(EDirectionalHitState::HitFromBack):

		Section = FName("FromBack");
		UE_LOG(LogTemp, Display, TEXT("Hit From Back"));
		break;

	case(EDirectionalHitState::HitFromFront):
		Section = FName("FromFront");
		UE_LOG(LogTemp, Display, TEXT("Hit From Front"));
		break;

	case(EDirectionalHitState::HitFromRight):
		Section = FName("FromRight");
		UE_LOG(LogTemp, Display, TEXT("Hit From Right"));
		break;

	case(EDirectionalHitState::HitFromLeft):
		Section = FName("FromLeft");
		UE_LOG(LogTemp, Display, TEXT("Hit From Left"));
		break;

	default:
		break;
	}

	return FName();
}

