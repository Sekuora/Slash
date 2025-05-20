// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/SlashCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Slash/Public/GameplayLogic/SlashFieldSystem.h"
#include "Breakables/Breakable.h"
#include "Interfaces/HitHandler.h"
#include "Combat/HitNexus.h"
#include "NiagaraComponent.h"

/**
* AWeapon derives from AItem
*/
AWeapon::AWeapon()
{
	// Hitbox
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit Ray"));
	HitBox->SetupAttachment(GetRootComponent());

	HitBox->SetGenerateOverlapEvents(true);
	HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	HitBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	HitBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Overlap);
	HitBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	HitBox->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));

	// Hit Rays
	HitRayStart = CreateDefaultSubobject<USceneComponent>(TEXT("Trace Start"));
	HitRayStart->SetupAttachment(GetRootComponent());

	HitRayEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Trace End"));
	HitRayEnd->SetupAttachment(GetRootComponent());

	// Sphere
	Sphere->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));


	// Item Mesh
	ItemMesh->SetGenerateOverlapEvents(false);

}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	// Bind HitBox to Function on BeginPlay
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnHitBoxOverlap);

}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{

	SetOwner(NewOwner);
	SetInstigator(NewInstigator);

	// Tie Weapon to socket
	TieToSocket(InParent, InSocketName);
	ItemState = EItemState::Equipped;
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation(), 0.3f);
	}

	// Disable Sphere collision
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}
	// Deactivate ItemEffect on Equip
	if (ItemEffect)
	{
		ItemEffect->Deactivate();
	}
}

void AWeapon::TieToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}


void AWeapon::OnSphereOverlap(UPrimitiveComponent* OnComponentBeginOverlap, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Call Parent function and pass parameters from overriden one
	Super::OnSphereOverlap(OnComponentBeginOverlap, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Call Parent function and pass parameters from overriden one
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}


void AWeapon::OnHitBoxOverlap(UPrimitiveComponent* OnComponentBeginOverlap, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	

	// Hit Rays Location
	const FVector Start = HitRayStart->GetComponentLocation();
	const FVector End = HitRayEnd->GetComponentLocation();

	// Add and iterate trough actors to ignore array
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	FHitResult Hit;

	// Call Box Trace Single Method to perform the box trace
	UKismetSystemLibrary::BoxTraceSingle
	(
		this, Start,End,FVector(5.f, 5.f, 5.f), HitRayStart->GetComponentRotation(), ETraceTypeQuery::TraceTypeQuery1,
		false, ActorsToIgnore, EDrawDebugTrace::None, Hit, true
	);

	TObjectPtr<AActor> HitActor = Hit.GetActor();

	if (HitActor)
	{
		if (GetOwner() == HitActor)
		{
			return;
		}

		// Ignore Actors of enemy faction to avoid enemies firendly fire.
		if (GetOwner()->ActorHasTag(TEXT("Faction.Enemy")) && HitActor->ActorHasTag(TEXT("Faction.Enemy")))
		{
			UE_LOG(LogTemp, Display, TEXT("Same Faction Actors Hit: Returning"));
			return;
		}
		
		// Apply Damage
		UGameplayStatics::ApplyDamage(Hit.GetActor(), Damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());

		IHitHandler* HitHandler = Cast<IHitHandler>(HitActor);
		if (HitHandler)
		{

			HitHandler->Execute_GetHit(HitActor, Hit.ImpactPoint);
		}
		

		IgnoreActors.AddUnique(Hit.GetActor());

		if (HitActor.IsA<ABreakable>())
		{
			SpawnFieldSystem(Hit.ImpactPoint);
		}

		
	}
}

void AWeapon::SpawnFieldSystem(const FVector& FieldLocation)
{

	if (!FieldSystem)
	{
		UE_LOG(LogTemp, Log, TEXT("Missing Field System"));
		return;
	}

	// Spawn Parameters
	FActorSpawnParameters FieldSpawnParameters;
	FieldSpawnParameters.Owner = this;
	FieldSpawnParameters.Name = NAME_None;

	// Convert FVector to Ftransform
	FTransform FieldTransform = UKismetMathLibrary::Conv_VectorToTransform(FieldLocation);
	
	// Spawn Field System
	AActor* SpawnedField = GetWorld()->SpawnActor<ASlashFieldSystem>(FieldSystem, FieldTransform, FieldSpawnParameters);
}