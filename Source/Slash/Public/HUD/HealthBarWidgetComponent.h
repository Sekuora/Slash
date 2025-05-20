// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarWidgetComponent.generated.h"


class UHealthBarUserWidget;

/**
 * 
 */
UCLASS()
class SLASH_API UHealthBarWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	
	UHealthBarWidgetComponent();

	/**
	* @brief Updates the health bar widget value for health percent.
	* 
	* Pass the current HealthPercent to the health progress bar.
	*/
	void UpdateHealthBar(float HealthPercent);

protected:
	virtual void BeginPlay() override;

private:
	
	/** 
	* 
	*/
	UPROPERTY()
	TObjectPtr<UHealthBarUserWidget> HealthBar;

};

