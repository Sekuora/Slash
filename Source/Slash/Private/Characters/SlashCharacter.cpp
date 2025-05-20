// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/SlashCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "Components/BoxComponent.h"
#include "Combat/CombatMontages.h"
#include "GameplayLogic/Components/MontagePlayerComponent.h"
#include "Combat/HitNexus.h"
#include "GameplayLogic/Components/DirectionalHitReactComponent.h"
#include "GameplayLogic/Components/HealthComponent.h"
#include "HUD/PlayerHUD.h"
#include "HUD/PlayerOverlayUserWidget.h"
#include "PlayerDataSubsystem.h"




ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 369.f, 0.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm")); 
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 250.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");


	// Collisions

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	MontagePlayer = CreateDefaultSubobject<UMontagePlayerComponent>(TEXT("MontagePlayerComponent"));
	MontagePlayer->SetMeshComponent(GetMesh());

	CombatMontages = CreateDefaultSubobject<UCombatMontages>(TEXT("CombatMontages"));

	HitNexus = CreateDefaultSubobject<UHitNexus>(TEXT("HitNexus"));
	
	HitReact = CreateDefaultSubobject<UDirectionalHitReactComponent>(TEXT("HitReact"));
	HitReact->SetOwnerActor(this);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerDataSubsystem = GetGameInstance()->GetSubsystem<UPlayerDataSubsystem>();

	if (PlayerDataSubsystem)
	{
		PlayerDataSubsystem->OnCollectedItem.AddDynamic(this, &ASlashCharacter::OnCollectedItemEvent);
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());

			if (PlayerHUD)
			{
				PlayerWidget = PlayerHUD->GetPlayerOveralyWidget();
				PlayerWidget->SetHealthBarPercent(HealthComponent->GetNormalizedHealthPercent());
				PlayerWidget->SetStaminaBarPercent(1.f);
				PlayerWidget->SetCoins(CurrentGoldAmount);
				PlayerWidget->SetSouls(CurrentSoulsAmount);
				
			}

			if (InputMapping)
			{
				InputSystem->AddMappingContext(InputMapping, 0);
			}
		}
	}

	Tags.Add(SlashCharacterTag);
}

void ASlashCharacter::Move(const FInputActionValue& Value)
{
	if (HealthComponent->GetHealthState() == EHealthState::Dead) { return; }
	
	
	if (TrackingState == ETrackingState::Ready)
	{
		TrackingState = ETrackingState::Moving;
	}
	
	if (TrackingState != ETrackingState::Moving)
	{
		return;
	}

	const FVector2D MovementVector = Value.Get<FVector2d>();

	const FRotator Rotation = Controller->GetControlRotation(); // Returns the current controller rotation, if controller exists, else returns zero rotation vector
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f); // Only takes the rotation from the yaw, to align player to camera rotation vector

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // Gather the x axis of the yaw axis
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); // Gather the y axis of the yaw axis

	// If attacking don't execute rest of snippet, stand still
	if (ActionState != EActionState::Ready) return;

	if (Controller && (MovementVector.X != 0) || (Controller && MovementVector.Y != 0))
	{
		AddMovementInput(ForwardDirection, MovementVector.Y); // Forward Movement
		AddMovementInput(RightDirection, MovementVector.X); // Right Movement
	}
}

void ASlashCharacter::Look(const FInputActionValue& Value)
{

	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller)
	{
		/** */
		AddControllerPitchInput(LookAxisVector.Y); // Add rotation about Y axis from mouse input
		AddControllerYawInput(LookAxisVector.X);	// Add rotation about X axis from mouse input

	}
}

void ASlashCharacter::EquipWeapon(const FInputActionValue& Value)
{
	if (HealthComponent->GetHealthState() == EHealthState::Dead) { return; }

	// On Equip check if overlapping item is from type AWeapon and return a cast to AWeapon if true 
	TObjectPtr<AWeapon> OverlappingWeapon = Cast<AWeapon>(OverlappingItem);

	if (OverlappingWeapon)
	{
		// Call the AWeapon's equip method, pass this character's mesh and socket name as parameters
		
		OverlappingWeapon->Equip(GetMesh(), FName("hand_r_socket"), this, this);


		CharacterState = ECharacterState::OneHanded;

		// Store Equipped Item to character current weapon
		OverlappingItem = nullptr;
		Weapon = OverlappingWeapon;
	}
	else
	{
		if (CanDisarm())
		{
			MontagePlayer->PlayMontageSection(CombatMontages->GetEquipWeaponMontage(), FName("Unequip"));
			CharacterState = ECharacterState::Unarmed;
			ActionState = EActionState::ToggleEquip;
		}
		else if (CanArm())
		{
			MontagePlayer->PlayMontageSection(CombatMontages->GetEquipWeaponMontage(), FName("Equip"));
			CharacterState = ECharacterState::OneHanded;
			ActionState = EActionState::ToggleEquip;
		}
	}
}

void ASlashCharacter::Attack(const FInputActionValue& Value)
{
	if (HealthComponent->GetHealthState() == EHealthState::Dead) { return; }

	if (CanAttack())
	{
		if (!MontagePlayer)
		{
			return;
		}

		MontagePlayer->RandomizeMontageSection(MontagePlayer->CollectMontageSectionNames(CombatMontages->GetAttackMontage()));
		MontagePlayer->PlayMontageSection(CombatMontages->GetAttackMontage(), MontagePlayer->GetCurrentMontageSection());

		ActionState = EActionState::Attacking;
	}
}

void ASlashCharacter::Dodge(const FInputActionValue& Value)
{
	/*if (HealthComponent->GetHealthState() == EHealthState::Dead) { return; }

	if (TrackingState == ETrackingState::Moving || TrackingState == ETrackingState::Ready)
	{
		TrackingState = ETrackingState::Dodging;
	}

	if(TrackingState != ETrackingState::Dodging)
	{
		return;
	}


	UE_LOG(LogTemp, Display, TEXT("DodgeTriggered!"));
	if (!MontagePlayer)
	{
		return;
	}

	if (ActionState != EActionState::Attacking && ActionState == EActionState::Ready)
	{
		MontagePlayer->RandomizeMontageSection(MontagePlayer->CollectMontageSectionNames(CombatMontages->GetDodgeMontage()));
		MontagePlayer->PlayMontageSection(CombatMontages->GetDodgeMontage(), MontagePlayer->GetCurrentMontageSection());

	}*/
	
}

void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::Ready;

	if (!Weapon) { return; }
	// Flush Ignore Actors Array
	Weapon->GetIgnoreActors().Empty();
	UE_LOG(LogTemp, Display, TEXT("Weapon IgnoreActors Emptied"));
}

// Items Data

void ASlashCharacter::OnCollectedItemEvent(const int32& Amount, const FName& ID)
{

	if (ID == FName("Treasure"))
	{
		CurrentGoldAmount += Amount;

		PlayerWidget->SetCoins(CurrentGoldAmount);
	}
	else if (ID == FName("Souls"))
	{
		CurrentSoulsAmount += Amount;

		HealthComponent->Heal(Amount);

		PlayerWidget->SetHealthBarPercent(HealthComponent->GetNormalizedHealthPercent());

		PlayerWidget->SetSouls(CurrentSoulsAmount);
	}

}

bool ASlashCharacter::CanAttack()
{
	return ActionState == EActionState::Ready &&
		CharacterState != ECharacterState::Unarmed;
}

bool ASlashCharacter::CanDisarm()
{

	return CharacterState == ECharacterState::OneHanded &&
		ActionState == EActionState::Ready;
}


bool ASlashCharacter::CanArm()
{
	return CharacterState == ECharacterState::Unarmed &&
		ActionState == EActionState::Ready &&
		Weapon;
}

void ASlashCharacter::TrackingStateReady()
{
	TrackingState = ETrackingState::Ready;
}

void ASlashCharacter::Disarm()
{
	if (Weapon)
	{
		Weapon->TieToSocket(GetMesh(), FName("spine_socket"));

	}
}

void ASlashCharacter::Arm()
{
	if (Weapon)
	{
		Weapon->TieToSocket(GetMesh(), FName("hand_r_socket"));
	}
}

void ASlashCharacter::onGearEnd()
{
	ActionState = EActionState::Ready;
}

void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
	
			
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);

		Input->BindAction(JumpAction, ETriggerEvent::Started, this, &ASlashCharacter::Jump);
		Input->BindAction(EquipAction, ETriggerEvent::Started, this, &ASlashCharacter::EquipWeapon);
		Input->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Attack);
		Input->BindAction(DodgeAction, ETriggerEvent::Started, this, &ASlashCharacter::Dodge);
		
	}
}

void ASlashCharacter::Jump()
{
	if (HealthComponent->GetHealthState() == EHealthState::Dead) { return; }

	if (ActionState != EActionState::Attacking && ActionState == EActionState::Ready)
	{
		Super::Jump();
	}
}

void ASlashCharacter::ToggleWeaponCollision(ECollisionEnabled::Type CollisionEnabled)
{
	if (Weapon && Weapon->GetHitBox())
	{
		Weapon->GetHitBox()->SetCollisionEnabled(CollisionEnabled);

	}
}

void ASlashCharacter::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (ActionState != EActionState::Disabled)
	{
		ActionState = EActionState::BeingHit;
	}
	else
	{
		return;
	}
	ToggleWeaponCollision(ECollisionEnabled::NoCollision);
	

	if (!HitReact || !HitNexus || !MontagePlayer || !CombatMontages)
	{
		return;
	}

	//
	// Get Hit Angle
	//

	// Fetch Theta from Impact Point

	double Theta = HitReact->FetchThetaFromHit(ImpactPoint);

	// Determine Hit React Section based on Mapped Hit Direction

	const FName& HitReactSection = HitReact->SelectHitReactSection(Theta);

	// Play Hit React Montage
	MontagePlayer->PlayMontageSection(CombatMontages->GetHitReactMontage(), FName(HitReactSection));

	// Basic Hit - Only spawns particles
	UE_LOG(LogTemp, Display, TEXT("Got Hit"));
	HitNexus->BasicHit(ImpactPoint);

	// On Death
	const FName& Death = "Death";

	if (HealthComponent->GetNormalizedHealthPercent() == 0.0f)
	{
		MontagePlayer->PlayMontageSection(CombatMontages->GetDeathMontage(), FName(Death));

		Tags.Add(FName("Death"));
		ActionState = EActionState::Disabled;
	
	}

}

float ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Update Health Component
	if (HealthComponent)
	{
		HealthComponent->ReceiveDamage(DamageAmount);

		// Update health state: When Life is lesser or equal to 0 HealthState = Dead
		HealthComponent->UpdateHealthState();

		PlayerWidget->SetHealthBarPercent(HealthComponent->GetNormalizedHealthPercent());

	}


	return DamageAmount;
}