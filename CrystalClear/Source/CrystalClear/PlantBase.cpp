// Fill out your copyright notice in the Description page of Project Settings.


#include "PlantBase.h"
#include "WaterableComponent.h"

APlantBase::APlantBase() {
	mWaterableComponent = CreateDefaultSubobject<UWaterableComponent>(TEXT("WaterableComponent"));
	mPlantMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlantMeshComponent"));
}
void APlantBase::BeginPlay()
{
	mWaterableComponent->mPlant = this;
	mPlantMesh->SetStaticMesh(mPlantStageMeshes[mPlantStageIndex]);
}
void APlantBase::Grow(float rate)
{
	if (mPlantStageIndex >= mPlantStages.Num()) {
		//Stop growing
		
	}
	else {
		//Do growth
		//base growth rate times the percentage of water the plant has
		mPlantStages[mPlantStageIndex].mCurrentGrowth += mBaseGrowthRate * (mWaterableComponent->mWaterLevel / mWaterableComponent->mMaxWaterLevel);
		if (mPlantStages[mPlantStageIndex].mCurrentGrowth >= mPlantStages[mPlantStageIndex].mRequiredGrowth) {
			//Next stage

			
			mPlantStageIndex++;
			mPlantMesh->SetStaticMesh(mPlantStageMeshes[mPlantStageIndex]);
		}
	}
	
}
