// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "SoilBase.generated.h"

/**
 * 
 */
UCLASS()
class CRYSTALCLEAR_API ASoilBase : public AInventoryItem
{
	GENERATED_BODY()
protected:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	ASoilBase();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soil")
		class UWaterableComponent* mWaterableComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soil")
		class APlantBase* mPlant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soil")
		class ASoilContainer* mContainer;

	virtual void GetPlant(APlantBase* plant);
	virtual void GetContainer(ASoilContainer* container);

	virtual void PickedUp() override;

	virtual void Grow();

	virtual void Use1() override;
	
};
