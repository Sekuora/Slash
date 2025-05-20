  // Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/WeaponsCore.h"
#include "Items/Weapons/Weapon.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"


 // Constructor
UWeaponsCore::UWeaponsCore()
{
	PrimaryComponentTick.bCanEverTick = false;

	//DetailsPanelExcludedCategories.Add("Weapon Management");
}

// Begin Play
void UWeaponsCore::BeginPlay()
{
	Super::BeginPlay();
}

// Tick Component
void UWeaponsCore::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UWeaponsCore::ToggleWeaponCollision(ECollisionEnabled::Type CollisionEnabled)
{
	if (ActiveWeapon && ActiveWeapon->GetHitBox())
	{
		ActiveWeapon->GetHitBox()->SetCollisionEnabled(CollisionEnabled);

	}
}

void UWeaponsCore::DestroyActiveWeapon(float ActiveWeaponLifeSpan)
{
	if (ActiveWeapon)
	{
		ActiveWeapon->SetLifeSpan(ActiveWeaponLifeSpan);
	}
}

//
// Weapon Functions
//

TObjectPtr<AWeapon> UWeaponsCore::SpawnWeapon(TSubclassOf<AWeapon> WeaponToSpawn)
{
	if (!WeaponToSpawn)
	{
		return nullptr;
	}

	TObjectPtr<AWeapon> SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponToSpawn, GetOwner()->GetTransform());

	// Return the spawned actor explicitly, otherwise it is inaccesible
	return SpawnedWeapon;
}


void UWeaponsCore::EquipWeapon(TObjectPtr<AWeapon> WeaponToEquip, USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{

	WeaponToEquip->SetOwner(NewOwner);
	WeaponToEquip->SetInstigator(NewInstigator);

	// Tie Weapon to socket

	AttachToSocket(WeaponToEquip, InParent, InSocketName);
	WeaponToEquip->SetItemState(EItemState::Equipped);

	if (WeaponToEquip->GetEquipSound())
	{
		UGameplayStatics::PlaySoundAtLocation(this, WeaponToEquip->GetEquipSound(), WeaponToEquip->GetActorLocation(), 0.3f);
	}

	// Disable Sphere collision
	if (WeaponToEquip->GetSphere())
	{
		WeaponToEquip->GetSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}
	// Deactivate ItemEffect on Equip
	if (WeaponToEquip->GetItemEffect())
	{
		WeaponToEquip->GetItemEffect()->Deactivate();
	}
}

void UWeaponsCore::AttachToSocket(TObjectPtr<AWeapon> WeaponToAttach, USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	WeaponToAttach->GetItemMesh()->AttachToComponent(InParent, TransformRules, InSocketName);
}

void UWeaponsCore::ToggleActiveWeaponCollision(ECollisionEnabled::Type CollisionEnabled)
{
	if (ActiveWeapon && ActiveWeapon->GetHitBox())
	{
		ActiveWeapon->GetHitBox()->SetCollisionEnabled(CollisionEnabled);

	}
}