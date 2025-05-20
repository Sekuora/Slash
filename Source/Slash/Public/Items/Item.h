// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UPlayerDataSubsystem;

enum class EItemState : uint8
{
	Hovering,
	Equipped
};

UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()

public:
	AItem();

	virtual void Tick(float DeltaTime) override;	

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Player Data");
	UPlayerDataSubsystem* PlayerDataSubsystem;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FName ItemID;

	// Sine Function Values

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float Amplitude = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters");
	float Rate = 5.f;

	// Rotate Speed Values

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Rotation Parameters");
	float xRotateSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Rotation Parameters");
	float yRotateSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Rotation Parameters");
	float zRotateSpeed = 0.f;

	// Item Movement Functions

	UFUNCTION(BlueprintPure, Category = "Item Movement Functions")
	float TransformSin();

	UFUNCTION(BlueprintPure, Category = "Item Movement Functions")
	float TransformCosin();

	UFUNCTION(BlueprintCallable)
	void rotateItem();

	template<typename T>
	T Avg(T First, T Second);

	/** Item StaticMesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly);
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	/** Item Sphere Component */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	EItemState ItemState = EItemState::Hovering;

	/** On Begin Overlap Delegate */
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OnComponentBeginOverlap, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	/** On End Overlap Delegate */
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Components | Visual Effects");
	TObjectPtr<UNiagaraComponent> ItemEffect;


private:

	/** Gather runtime's total running time */
	UPROPERTY(VisibleAnywhere, Category = "Sine Parameters");
	float RunningTime;

public:
	FORCEINLINE EItemState GetItemState() { return ItemState; };
	FORCEINLINE void SetItemState(EItemState InItemState) { ItemState = InItemState; };

};

template<typename T>
inline T AItem::Avg(T First, T Second)
{
	return (First + Second) / 2;
}
