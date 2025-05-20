// Copyright 2024 Fofaya Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Field/FieldSystemActor.h"
#include "SlashFieldSystem.generated.h"

class UCullingField;
class USphereComponent;
class UFieldSystemComponent;
class UStaticMeshComponent;
class UFieldSystemMetaDataFilter;
class URadialVector;
class URadialFalloff;
class ABreakable;

/** 
* @brief Field System to be used within the Slash Game Framework.
*/
UCLASS()
class SLASH_API ASlashFieldSystem : public AFieldSystemActor
{
	GENERATED_BODY()

public:
	ASlashFieldSystem();

protected:

	virtual void BeginPlay() override;
	/*
	* Field System Objects
	*/
	UPROPERTY(VisibleAnywhere, Category = "Field System")
	TObjectPtr<UStaticMeshComponent> VisibilitySphere;

	UPROPERTY(VisibleAnywhere, Category = "Field System")
	TObjectPtr<USphereComponent> RadiusSphere;

	UPROPERTY(VisibleAnywhere, Category = "Field System")
	TObjectPtr<URadialVector> RadialVector;

	UPROPERTY(VisibleAnywhere, Category = "Field System")
	TObjectPtr<URadialFalloff> RadialFalloff;

	UPROPERTY(VisibleAnywhere, Category = "Field System")
	TObjectPtr<UCullingField> CullingField;

	UPROPERTY(VisibleAnywhere, Category = "Field System")
	TObjectPtr<UFieldSystemMetaDataFilter> MetaDataFilter;

	UPROPERTY(VisibleAnywhere, Category = "Field System")
	TObjectPtr<UFieldSystemComponent> FieldSystemRoot;

	/*
	* Field System Properties
	*/

	/** Field System Radius */
	UPROPERTY(VisibleAnywhere, Category = "Field System Properties")
	float FieldRadius = 100.f;

	/** Radial Falloff Magnitude */
	UPROPERTY(EditDefaultsOnly, Category = "Field System Properties")
	float RadialFalloffMagnitude = 500000.f;

	/** Radial Vector Magnitude */
	UPROPERTY(EditDefaultsOnly, Category = "Field System Properties")
	float RadialVectorMagnitude = 800.f;

	UPROPERTY(EditDefaultsOnly, Category = "Field System Properties")
	float LifeSpan = 1.f;

	/** Toggle Field Visibility */
	UPROPERTY(EditAnywhere, Category = "Field System Properties")
	bool isFieldVisible = false;

private:
	/*
	* Field System Functions
	*/ 

	/**
	* Creates a field system, define forces inside.
	* Forces must be created as NewObject to work at runtime.
	*/
	UFUNCTION()
	void CreateField();

	/**
	* Dynamically adjust visibility sphere to follow radius sphere size
	*/ 
	UFUNCTION()
	void ToggleFieldVisibility();


public:
	//FORCEINLINE float LifeSpan GetLifeSpan() { return LifeSpan; };

};