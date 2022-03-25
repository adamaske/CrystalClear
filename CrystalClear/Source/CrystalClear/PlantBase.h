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
	UPROPERTY(EditAnywhere, Category = "Plant")
	float mCurrentGrowth = 0;
	UPROPERTY(EditAnywhere, Category = "Plant")
	float mRequiredGrowth = 100;
};
UCLASS()
class CRYSTALCLEAR_API APlantBase : public AInventoryItem
{
	GENERATED_BODY()
public:
	APlantBase();
protected:
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant", meta = (TitleProperty = "Plant"))
		TArray<FPlantStage> mPlantStages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant", meta = (TitleProperty = "Plant"))
		class UStaticMeshComponent* mPlantMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
		int mPlantStageIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
		float mBaseGrowthRate = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
		TArray<class UStaticMesh*> mPlantStageMeshes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
		class UWaterableComponent* mWaterableComponent;
	void Grow(float rate);
};
