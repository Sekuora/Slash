// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerOverlayUserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerOverlayUserWidget::SetHealthBarPercent(float Percent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Percent);
	}
}

void UPlayerOverlayUserWidget::SetStaminaBarPercent(float Percent)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(Percent);
	}
}

void UPlayerOverlayUserWidget::SetCoins(int32 Coins)
{
	if (Coin_Text)
	{

		Coin_Text->SetText(FText::FromString(FString::Printf(TEXT("%d"), Coins)));
	}
}

void UPlayerOverlayUserWidget::SetSouls(int32 Souls)
{
	if (Soul_Text)
	{
		Soul_Text->SetText(FText::FromString(FString::Printf(TEXT("%d"), Souls)));
	}
}
