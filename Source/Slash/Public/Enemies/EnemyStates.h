#pragma once

#include "CoreMinimal.h"
#include "EnemyStates.generated.h"

UENUM(BlueprintType) // UENUM exposes enums to blueprint
enum class ENavigationStates : uint8
{
	Idle,
	PatrolMode,
	ChaseMode,
	CombatMode,
	AttackMode,
	TargetExitCombat,
	FindNewTarget,
	InPatrolRange,
};

UENUM(BlueprintType) // UENUM exposes enums to blueprint
enum class ETargetStates : uint8
{
	HasCombatTarget,
	WaitingCombatTarget,
};

UENUM(BlueprintType) // UENUM exposes enums to blueprint
enum class ETargetTransitiveState : uint8
{
	ToLeft,
	ToRight,
};



UENUM(BlueprintType) // UENUM exposes enums to blueprint
enum class ESightRange : uint8
{
	CombatModeRange,
	SightRange,
	AttackRange,
};

UENUM(BlueprintType) // UENUM exposes enums to blueprint
enum class ETransitiveStates : uint8
{
	HitFromBack,
	HitFromFront,
	HitFromRight,
	HitFromLeft,
};

