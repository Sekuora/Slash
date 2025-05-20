// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Item.h"
#include "Debug/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Characters/SlashCharacter.h"
#include "PlayerDataSubsystem.h"
#include "NiagaraComponent.h"

#define THIRTY 30

// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	// Item Mesh
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;


	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	// Sphere Object attached to root component
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


	// Create Interactable Effects
	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Component"));
	ItemEffect->SetupAttachment(GetRootComponent());
	ItemEffect->SetAutoActivate(true);

}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	// Reference to world where actor exists
	UWorld* World = GetWorld();

	// Save this actor location
	FVector ActorLocation = GetActorLocation();

	// Save Actor Forward Vector
	FVector ForwardVector = GetActorForwardVector();

	// Add Callback to OnComponentBeginOverlap Event, call OnSphereOverlap method
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	
	// EndOverlap Event Callback, call OnSphereEndOverlap method
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);

	PlayerDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UPlayerDataSubsystem>();

}

void AItem::Tick(float DeltaTime)	// Called every frame
{
	Super::Tick(DeltaTime);

	// Add delta time to the Running Time variable
	RunningTime += DeltaTime;

	if (ItemState == EItemState::Hovering)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformSin()));
	}
}

float AItem::TransformSin()
{
	float DeltaSin = Amplitude * FMath::Sin(RunningTime * Rate);
	return DeltaSin;
}

float AItem::TransformCosin()
{
	float DeltaCos = Amplitude * FMath::Cos(RunningTime * Rate);
	return DeltaCos;
}

void AItem::rotateItem()
{	
	// Assign World Delta Seconds to local Variable DeltaTime to ensure framerate independence
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	// Control speed through rotation speed parameters: Instance independent
	AddActorWorldRotation(FRotator(xRotateSpeed, yRotateSpeed, zRotateSpeed) * DeltaTime);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OnComponentBeginOverlap, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Gather other actor reference
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	
	if (SlashCharacter)
	{
		SlashCharacter->SetOverlappingItem(this);		
	}
}

/** Define what happens when another actor ends an existing overlap with item's sphere radius */
void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// If ends overlap - set item to null
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	if (SlashCharacter)
	{
		
		SlashCharacter->SetOverlappingItem(nullptr);
	}
}

