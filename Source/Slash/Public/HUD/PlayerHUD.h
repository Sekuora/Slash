// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"


class UPlayerOverlayUserWidget;

/**
 * 
 */
UCLASS()
class SLASH_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UPlayerOverlayUserWidget> PlayerOverlay;

	UPROPERTY()
	UPlayerOverlayUserWidget* PlayerOverlayWidget;

public:
	FORCEINLINE UPlayerOverlayUserWidget* GetPlayerOveralyWidget() const { return PlayerOverlayWidget; }
};
