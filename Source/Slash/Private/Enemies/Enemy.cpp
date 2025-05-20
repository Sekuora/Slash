// Copyright 2024 Fofaya Games. All Rights Reserved.


#include "Enemies/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Slash/Private/Debug/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayLogic/Components/HealthComponent.h"
#include "HUD/HealthBarWidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Debug/DebugMacros.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameplayLogic/Components/MontagePlayerComponent.h"
#include "GameplayLogic/Components/DirectionalHitReactComponent.h"
#include "Combat/CombatMontages.h"
#include "Combat/CombatComponent.h"
#include "Combat/WeaponsCore.h"
#include "Items/Weapons/Weapon.h"
#include "Combat/HitNexus.h"
#include "Items/Lootable.h"


//
// Constructor
//

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	InitialLifeSpan = 0.f;

	// Mesh

	GetMesh()->SetGenerateOverlapEvents(true);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);

	GetMesh()->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));

	// CapsuleComponent

	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	GetCapsuleComponent()->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));

	// Actor Components

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	// Widget Components

	HealthBarWidget = CreateDefaultSubobject<UHealthBarWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	HealthBarWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HealthBarWidget->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	// Character Movement Component

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Montages

	MontagePlayer = CreateDefaultSubobject<UMontagePlayerComponent>(TEXT("MontagePlayerComponent"));
	MontagePlayer->SetMeshComponent(GetMesh());
	
	CombatMontages = CreateDefaultSubobject<UCombatMontages>(TEXT("CombatMontages"));

	// Pawn Sensing Component

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensing->SightRadius = PawnSightRadius;
	PawnSensing->SetPeripheralVisionAngle(45.f);

	// Directional Hit React Component

	DirectionalHitReact = CreateDefaultSubobject<UDirectionalHitReactComponent>(TEXT("DirectionalHitReact"));
	DirectionalHitReact->SetOwnerActor(this);

	// Combat

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	WeaponsCore = CreateDefaultSubobject<UWeaponsCore>(TEXT("WeaponsCore"));

	HitNexus = CreateDefaultSubobject<UHitNexus>(TEXT("HitNexus"));


	// Faction Tag
	Tags.Add(FactionTag);
}


//
// Begin Play
//

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Set Character Values

	GetCharacterMovement()->MaxWalkSpeed = PatrolWalkSpeed;

	// Set States

	TargetState = ETargetStates::WaitingCombatTarget;

	NavigationState = ENavigationStates::Idle;

	HealthComponent->SetHealthState(EHealthState::Alive);

	// Disable auto blend for dead state montage to avoid going back to idle states, etc.

	MontagePlayer->SetMontageBlendOut(CombatMontages->GetDeathMontage(), false, 0.f);

	// Set UI

	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}

	// Set AI Controller

	AIController = Cast<AAIController>(GetController());

	if (PawnSensing)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn Sensing"));
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::WhenSeePawn);
	}
	
	// Start with a random patrol target

	RandomizeNextPatrolTarget();
	MoveToTarget(CurrentPatrolTarget);
	

	LookAtTargetPitch = 0.f;

	// SetActiveWeapon as the default weapon passed in weapons core

	WeaponsCore->SetActiveWeapon(WeaponsCore->SpawnWeapon(WeaponsCore->GetDefaultWeapon()));

	if (WeaponsCore->GetActiveWeapon())
	{
		WeaponsCore->EquipWeapon(WeaponsCore->GetActiveWeapon(), GetMesh(), FName("hand_r_socket"), this, this);
	}
}

//
// Tick
// 

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HealthComponent->GetHealthState() == EHealthState::Alive) 
	{ 
		// Rename function to something like: UpdateRangeToCombatTarget
		// Actions that happen when combat target is set
		UpdateCombatTargetRadius();

		// Find patrol target continuosly when not in combat
		FindPatrolTarget();
	};
}

//
// Input Component
//

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//
// Section: Hit Handling
//

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{

	if (!DirectionalHitReact)
	{
		return;
	}

	//
	// Get Hit Angle
	//

	// Fetch Theta from Impact Point

	const double& Theta = DirectionalHitReact->FetchThetaFromHit(ImpactPoint);

	// Determine Hit React Section based on Mapped Hit Direction

	const FName& HitReactSection = DirectionalHitReact->SelectHitReactSection(Theta);

	//
	// Look At Target
	//

	// Rotator to look at target when hit.
	FRotator LookAtTarget = (CombatTarget->GetActorLocation() - GetActorLocation()).Rotation();

	// Set rotation and rotation speed according to conditions.
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), LookAtTarget, GetWorld()->GetDeltaSeconds(), LookAtTargetSpeed));

	// Set state to ready to allow for AttackIdleTimer retrigger
	CombatComponent->SetCombatState(ECombatState::Ready);

	//
	// HUD
	//

	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}

	//
	// Health State Managment
	//

	if (HealthComponent && HealthComponent->GetHealthState() == EHealthState::Alive)
	{
		if (!MontagePlayer || !CombatMontages)
		{
			return;
		}

		MontagePlayer->PlayMontageSection(CombatMontages->GetHitReactMontage(), FName(HitReactSection));
	}
	else
	{
		CombatComponent->WhenEntityDies();

		FVector Location = GetActorLocation();

		Location.X += FMath::RandRange(5.f, 15.f);
		Location.Y += FMath::RandRange(5.f, 15.f);
		Location.Z += 10.f;

		FActorSpawnParameters LootSpawnParameters;
		LootSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<ALootable>(SoulLoot, Location, GetActorRotation(), LootSpawnParameters);
	}

	// Hit sound effects

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}

	// Hit particle effects

	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ImpactPoint);
	}
}

//
// Section: Damage Handling / Health Component 
// 

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Update Health Component
	if (HealthComponent && HealthBarWidget)
	{
		HealthComponent->ReceiveDamage(DamageAmount);

		HealthBarWidget->UpdateHealthBar(HealthComponent->GetNormalizedHealthPercent());

		// Update health state: When Life is lesser or equal to 0 HealthState = Dead
		HealthComponent->UpdateHealthState();
	}

	CombatTarget = EventInstigator->GetPawn();
	TargetState = ETargetStates::HasCombatTarget;

	return DamageAmount;
}


//
// Section: AI Navigation - Combat Target
//


void AEnemy::WhenSeePawn(APawn* SeenPawn)
{
	if (TargetState == ETargetStates::HasCombatTarget || SeenPawn->ActorHasTag("Death"))
	{
		return;
	}
	
	


	// Clear Patrol Timer
	GetWorldTimerManager().ClearTimer(PatrolIdleTimer);


	CombatTarget = SeenPawn;
	// Set Target State
	TargetState = ETargetStates::HasCombatTarget;
	

	

	UE_LOG(LogTemp, Display, TEXT("Target State: Seen Pawn Assigned as combat target"));
}

void AEnemy::UpdateCombatTargetRadius()
{
	// HasCombatTarget - Guard Clause
	if (TargetState != ETargetStates::HasCombatTarget)
	{
		return;
	}

	

	//
	// On Target Enter Range
	//

	// When Target Enters PawnSightRadius
	if (InTargetRange(CombatTarget, PawnSightRadius) && NavigationState != ENavigationStates::ChaseMode)
	{
		

		// On Combat Radius Exit
		if (!InTargetRange(CombatTarget, CombatRadius))
		{
			UE_LOG(LogTemp, Display, TEXT("Chasing Mode Active"));
			NavigationState = ENavigationStates::ChaseMode;

			// Set Chase Walk Speed
			GetCharacterMovement()->MaxWalkSpeed = ChaseWalkSpeed;
		}

	}

	// When Target Enters Combat Radius
	if (InTargetRange(CombatTarget, CombatRadius) && NavigationState != ENavigationStates::CombatMode)
	{
		
		// On Attack Radius Exit
		if (!InTargetRange(CombatTarget, AttackRadius))
		{

			// Enter Combat Mode

			NavigationState = ENavigationStates::CombatMode;
			UE_LOG(LogTemp, Display, TEXT("Combat Mode Active"));

			GetCharacterMovement()->MaxWalkSpeed = CombatWalkSpeed;
		}
	}

	// When Target Enters Attack Radius
	if (InTargetRange(CombatTarget, AttackRadius) && NavigationState != ENavigationStates::AttackMode)
	{	
		// Enable Attack Mode UI

		if (!HealthBarWidget)
		{
			HealthBarWidget->SetVisibility(true);
		}

		NavigationState = ENavigationStates::AttackMode;
		UE_LOG(LogTemp, Display, TEXT("Attack Mode Active"));

	}

	//
	// On Target Exit Range
	//

	// When Target Exits Pawn Sight Radius
	if ((!InTargetRange(CombatTarget, PawnSightRadius) || CombatTarget->ActorHasTag("Death")) && NavigationState != ENavigationStates::Idle)
	{
		// Disable Attack Mode UI
		if (HealthBarWidget)
		{
			HealthBarWidget->SetVisibility(false);
		}
		
		CombatTarget = nullptr;

		// Set Target State
		TargetState = ETargetStates::WaitingCombatTarget;

		// Set to idle - Will be set back to patrol after entering next patrol target radius
		UE_LOG(LogTemp, Display, TEXT("Idle Mode Set"));
		NavigationState = ENavigationStates::Idle;

		// Clear Attack Timer
		CombatComponent->StopAttackTimer();

		// Set patrol timer back after three seconds
		GetWorldTimerManager().SetTimer(PatrolIdleTimer, this, &AEnemy::PatrolIdleTimerComplete, 3.f);
	}

	// TODO: Separate conditions in other function.
	if (NavigationState == ENavigationStates::AttackMode && CombatComponent->GetCombatState() != ECombatState::Attacking)
	{
		CombatComponent->StartAttackTimer();
	}

	if (CombatComponent->GetCombatState() == ECombatState::Attacking)
	{
		AIController->StopMovement();

	}
	else
	{
		MoveToTarget(CombatTarget);
	}

	if (CombatTarget)
	{
		// Rotator to look at target when hit.
		FRotator LookAtTarget = (CombatTarget->GetActorLocation() - GetActorLocation()).Rotation();

		LookAtTargetPitch = LookAtTarget.Pitch;

		if (NavigationState == ENavigationStates::AttackMode && LookAtTarget.Pitch > 0 && TargetTransitiveState != ETargetTransitiveState::ToRight)
		{
			TargetTransitiveState = ETargetTransitiveState::ToRight;
			UE_LOG(LogTemp, Display, TEXT("Switch target to right"));
		}

		if (NavigationState == ENavigationStates::AttackMode && LookAtTarget.Pitch < 0 && TargetTransitiveState != ETargetTransitiveState::ToLeft)
		{
			TargetTransitiveState = ETargetTransitiveState::ToLeft;
			UE_LOG(LogTemp, Display, TEXT("Switch target to left"));
		}

		// Set rotation and rotation speed according to conditions.
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), LookAtTarget, GetWorld()->GetDeltaSeconds(), LookAtTargetSpeed));

	}


}

bool AEnemy::InTargetRange(AActor* ActorToTarget, double TargetableRadius)
{
	if (!ActorToTarget)
	{
		return false;
	}

	// Get length obtained from target actor to this actor
	const double DistanceToTarget = (ActorToTarget->GetActorLocation() - GetActorLocation()).Size();

	DRAW_SPHERE_SingleFrame(GetActorLocation(), FColor::Blue);
	DRAW_SPHERE_SingleFrame(ActorToTarget->GetActorLocation(), FColor::Red);

	return DistanceToTarget <= TargetableRadius;
}

//
// Section: AI Navigation / Patrol
//

void AEnemy::FindPatrolTarget()
{

	const float TimerRandomRange = FMath::RandRange(MinPatrolIdleTime, MaxPatrolIdleTime);

	// When entering a patrol target radius
	if (InTargetRange(CurrentPatrolTarget, PatrolTargetRadius))
	{
		// When Navigation State is set to any state below PatrolMode
		if (NavigationState < ENavigationStates::PatrolMode)
		{
			UE_LOG(LogTemp, Display, TEXT("Patrol Mode Set"));
			NavigationState = ENavigationStates::PatrolMode;
		}

		// Enter Patrol Mode
		if (NavigationState == ENavigationStates::PatrolMode)
		{
			GetCharacterMovement()->MaxWalkSpeed = PatrolWalkSpeed;

			RandomizeNextPatrolTarget();
			GetWorldTimerManager().SetTimer(PatrolIdleTimer, this, &AEnemy::PatrolIdleTimerComplete, TimerRandomRange);

		}
	}

}

void AEnemy::MoveToTarget(TObjectPtr<AActor> PatrolTarget)
{
	if (!AIController || !PatrolTarget)
	{
		return;
	}


	// Setup MoveRequest
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(PatrolTarget);
	MoveRequest.SetAcceptanceRadius(50.f);

	// Call MoveTo Function
	AIController->MoveTo(MoveRequest);

}

void AEnemy::RandomizeNextPatrolTarget()
{

	if (!PreviousPatrolTarget)
	{
		UE_LOG(LogTemp, Display, TEXT("PreviousPatrolTarget = InitialPatrolTarget"));
		PreviousPatrolTarget = InitialPatrolTarget;
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("PreviousPatrolTarget = CurrentPatrolTarget"));
		PreviousPatrolTarget = CurrentPatrolTarget;
	}

	// Create local array to add next valid targets from PatrolTargets array
	// Is valid if it is not equal to previous stored patrol target.
	TArray<TObjectPtr<AActor>> NextValidTargets;
	for (TObjectPtr<AActor> ValidTarget : PatrolTargets)
	{
		if (ValidTarget != PreviousPatrolTarget)
		{
			NextValidTargets.AddUnique(ValidTarget);
		}
	}

	// Check amount of valid patrol numbers
	const int32 PatrolTargetsAmount = NextValidTargets.Num();

	if (PatrolTargetsAmount > 0)
	{
		// Generate random target index number
		const int32 RandomTargetNumber = FMath::RandRange(0, PatrolTargetsAmount - 1);

		// Assign random target index as target
		TObjectPtr<AActor> RandomPatrolTarget = NextValidTargets[RandomTargetNumber];

		UE_LOG(LogTemp, Display, TEXT("Assigned Random Target"));
		// Store random patrol target as current patrol target.
		CurrentPatrolTarget = RandomPatrolTarget;
	}
}


void AEnemy::PatrolIdleTimerComplete()
{
	if (CurrentPatrolTarget)
	{
		MoveToTarget(CurrentPatrolTarget);
	}
}

