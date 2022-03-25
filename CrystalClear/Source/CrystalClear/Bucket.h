// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "Bucket.generated.h"

/**
 * 
 */
UCLASS()
class CRYSTALCLEAR_API ABucket : public AInventoryItem
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
		class APlantBase* mPlant;

	void GetSoil();
	void GrowPlant();
};
