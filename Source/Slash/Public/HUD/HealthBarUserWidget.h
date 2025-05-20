// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarUserWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class SLASH_API UHealthBarUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/** 
	* @brief HealhProgressBar Widget to bind to class.
	* 
	* Names of class variable and blueprint widget variable must match
	* in order to bind them.
	*/
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthProgressBar;

};
