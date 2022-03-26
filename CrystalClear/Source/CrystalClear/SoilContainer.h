// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "SoilContainer.generated.h"

/**
 * 
 */
UCLASS()
class CRYSTALCLEAR_API ASoilContainer : public AInventoryItem
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
		class APlantBase* mPlant;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
		class ASoilBase* mSoil;
	void GetSoil(ASoilBase* soil);
	bool GetPlant(APlantBase* plant);
	virtual void Use1() override;
};
