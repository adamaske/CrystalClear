// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "SoilPatch.generated.h"

/**
 * 
 */
UCLASS()
class CRYSTALCLEAR_API ASoilPatch : public AInventoryItem
{
	GENERATED_BODY()
protected:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soil")
		class UWaterableComponent* mWaterableComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soil")
		class APlantBase* mPlant;

	virtual void Grow();

	virtual void Use1() override;
	
};
