// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarWidgetComponent.h"
#include "HUD/HealthBarUserWidget.h"
#include "Components/ProgressBar.h"


void UHealthBarWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
}

UHealthBarWidgetComponent::UHealthBarWidgetComponent()
{
	this->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void UHealthBarWidgetComponent::UpdateHealthBar(float HealthPercent)
{

	if (HealthBar == nullptr)
	{
		HealthBar = Cast<UHealthBarUserWidget>(GetUserWidgetObject());
	}
	
	// Pass health percet value to healthbar percent value
	if (HealthBar && HealthBar->HealthProgressBar)
	{
		HealthBar->HealthProgressBar->SetPercent(HealthPercent);
	}

}

