#pragma once

// enum class: E to indicate Enum
UENUM(BlueprintType) // UENUM exposes enums to blueprint
enum class ECharacterState : uint8
{
	Unarmed,
	OneHanded,
	TwoHanded,
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	Ready,
	BeingHit,
	Attacking,
	ToggleEquip,
	Disabled,
	
};

UENUM(BlueprintType)
enum class ETrackingState : uint8
{
	Moving,
	Dodging,
	Ready
};
