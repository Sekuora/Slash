// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"




UENUM(BlueprintType)
enum class ECombatState : uint8
{
	Ready,
	Attacking,
	ToggleEquip,
};

UENUM(BlueprintType)
enum class ECombatTargetState : uint8
{
	ToLeft,
	ToRight,
};


/**
 * 
 */
class SLASH_API CombatStates
{
public:
	CombatStates();
	~CombatStates();
};


