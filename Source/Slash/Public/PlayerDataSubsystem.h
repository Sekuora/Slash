// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PlayerDataSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCollectedItem, const int32&, Amount, const FName&, ID);

/**
 * 
 */
UCLASS()
class SLASH_API UPlayerDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Bind Function Handlers
	UPROPERTY(BlueprintAssignable)
	FCollectedItem OnCollectedItem;

	// Broadcast
	UFUNCTION(BlueprintCallable)
	void Broadcast_OnCollectedItem(const int32& Amount, const FName& ID);


};
