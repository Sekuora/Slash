// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakables/Breakable.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PBDRigidsSolver.h"
#include "Items/Lootable.h"
#include "Components/CapsuleComponent.h"
#include "Chaos/ChaosGameplayEventDispatcher.h"
#include "Breakables/BreakableStates.h"


ABreakable::ABreakable()
{
	PrimaryActorTick.bCanEverTick = false;

	// Geometry Collection
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);

	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetNotifyBreaks(true);
	GeometryCollection->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));

	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	// Capsule Volume
	CapsuleVolume = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleVolume"));
	CapsuleVolume->SetupAttachment(GeometryCollection);

	CapsuleVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CapsuleVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	// Events
	GeometryCollection->OnChaosBreakEvent.AddDynamic(this, &ABreakable::HandleChaosBreak);

	// States
	BreakableState = EBreakableState::Intact;
}

void ABreakable::BeginPlay()
{
	Super::BeginPlay();
}

void ABreakable::HandleChaosBreak(const FChaosBreakEvent& BreakEvent)
{
	// UE_LOG(LogTemp, Log, TEXT("Chaos Break Event Ocurred"));
	if (GeometryCollection)
	{
		SetLifeSpan(5.f);
	}
}

void ABreakable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakable::GetHit_Implementation(const FVector& ImpactPoint)
{
	

	TObjectPtr<UWorld> World = GetWorld();

	if (!World || LootItems.Num() < 0  || BreakableState == EBreakableState::Broken)
	{
		return;
	}

	// Attempt to keep CapsuleVolume Attached to Geometry Collection
	CapsuleVolume->AttachToComponent(GeometryCollection, FAttachmentTransformRules::KeepWorldTransform);

	// LootItem Spawn Parameters
	FVector Location = GetActorLocation();
	FActorSpawnParameters LootSpawnParameters;
	LootSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Location.Z += ZSpawnOffset;

	// Random Loot Array Index Number
	const int32 RandomItemIndex = FMath::RandRange(0, (LootItems.Num() - 1));

	// Spawn Function Call
	World->SpawnActor<ALootable>(LootItems[RandomItemIndex], Location, GetActorRotation(), LootSpawnParameters);

	// Set State To Broken
	BreakableState = EBreakableState::Broken;

	if (HitSound)
	{
		// Play sound at hit location
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
	CapsuleVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

