// Copyright 2024 Fofaya Games. All Rights Reserved.

#include "GameplayLogic/SlashFieldSystem.h"
#include "Components/SphereComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Breakables/Breakable.h"
#include "Field/FieldSystemObjects.h"
#include "Field/FieldSystemComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASlashFieldSystem::ASlashFieldSystem()
{
	PrimaryActorTick.bCanEverTick = true;

	/*
	* Main Attributes
	*/

	// Field System Component
	SetRootComponent(FieldSystemComponent);

	// Radius Sphere
	RadiusSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereRadius"));
	RadiusSphere->SetupAttachment(GetRootComponent());
	RadiusSphere->SetSphereRadius(FieldRadius);


	// Visibility Sphere
	VisibilitySphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibilitySphere"));
	VisibilitySphere->SetupAttachment(RadiusSphere);

	VisibilitySphere->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	VisibilitySphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);


	MetaDataFilter = CreateDefaultSubobject<UFieldSystemMetaDataFilter>(TEXT("MetaDataFilter"));
	MetaDataFilter->SetMetaDataFilterType(EFieldFilterType::Field_Filter_Dynamic, Field_Object_Destruction, EFieldPositionType::Field_Position_CenterOfMass);

}

void ASlashFieldSystem::BeginPlay()
{
	Super::BeginPlay();
	
	ToggleFieldVisibility();

	CreateField();

	SetLifeSpan(LifeSpan);
}

void ASlashFieldSystem::CreateField()
{
	if (!MetaDataFilter)
	{
		UE_LOG(LogTemp, Log, TEXT("Missing Metadatafilter"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("FieldSystem Called"));

	float RadiusSphereCollisionSize = RadiusSphere->GetScaledSphereRadius();
	FVector RadiusSphereWorldLocation = RadiusSphere->GetComponentLocation();

	// Set Physic Forces
	RadialVector = NewObject<URadialVector>(this, TEXT("RadialVector"));
	RadialVector->SetRadialVector(RadialVectorMagnitude, RadiusSphereWorldLocation);

	RadialFalloff = NewObject<URadialFalloff>(this, TEXT("RadialFalloff"));
	RadialFalloff->SetRadialFalloff(RadialFalloffMagnitude, 0.f, 1.f, 0.f, RadiusSphereCollisionSize, RadiusSphereWorldLocation, Field_FallOff_None);

	CullingField = NewObject<UCullingField>(this, TEXT("CullingField"));
	CullingField->SetCullingField(RadialFalloff, RadialVector, Field_Culling_Outside);

	// Add transient fields with corresponding forces
	FieldSystemComponent->ApplyPhysicsField(true, Field_ExternalClusterStrain, nullptr, RadialFalloff);
	FieldSystemComponent->ApplyPhysicsField(true, Field_LinearVelocity, nullptr, CullingField);
}

void ASlashFieldSystem::ToggleFieldVisibility()
{
	if (!isFieldVisible)
	{
		VisibilitySphere->SetVisibility(false);
		return;
	}

	if (!RadiusSphere || !VisibilitySphere)
	{
		return;
	}

	VisibilitySphere->SetVisibility(true);
	// Adjust Visibility Sphere to Field Radius
	float VisibilitySphereScalar = 50.f;
	float VisibilitySphereScaleRatio = FieldRadius / VisibilitySphereScalar;
	FVector VisibilitySphereSize(VisibilitySphereScaleRatio, VisibilitySphereScaleRatio, VisibilitySphereScaleRatio);
	VisibilitySphere->SetRelativeScale3D(VisibilitySphereSize);
}



