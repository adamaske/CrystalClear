// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "PlantBase.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FPlantStage {
	GENERATED_BODY()
public:
	float mCurrentGrowth = 0;
	float mRequiredGrowth = 100;
};
UCLASS()
class CRYSTALCLEAR_API APlantBase : public AInventoryItem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
		TArray<FPlantStage> mPlantStages;
};
