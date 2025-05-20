// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDataSubsystem.h"

void UPlayerDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
}

void UPlayerDataSubsystem::Deinitialize()
{
}

void UPlayerDataSubsystem::Broadcast_OnCollectedItem(const int32& Amount, const FName& ID)
{
	OnCollectedItem.Broadcast(Amount, ID);
}
