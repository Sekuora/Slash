
// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD.h"
#include "HUD/PlayerOverlayUserWidget.h"

void APlayerHUD::BeginPlay()
{

	Super::BeginPlay();

	TObjectPtr<UWorld> World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && PlayerOverlay)
		{
			PlayerOverlayWidget = CreateWidget<UPlayerOverlayUserWidget>(Controller, PlayerOverlay);

			
			PlayerOverlayWidget->AddToViewport();
			
		}
	}


}
