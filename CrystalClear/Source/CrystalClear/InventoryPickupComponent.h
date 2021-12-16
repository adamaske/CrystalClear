// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponent.h"
#include "InventoryPickupComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYSTALCLEAR_API UInventoryPickupComponent : public UInteractableComponent
{
	GENERATED_BODY()
public:
	virtual void Interact(APlayerUnit* player) override;

	virtual void BeginPlay() override;
	void PickedUp();
	void Dropped();
	
	UPROPERTY(EditAnywhere)
		bool bActivateMeOnPickup = true;
};
