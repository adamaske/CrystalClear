// Fill out your copyright notice in the Description page of Project Settings.


#include "PlantBase.h"
#include "WaterableComponent.h"
#include "SoilBase.h"

APlantBase::APlantBase() {
	mWaterableComponent = CreateDefaultSubobject<UWaterableComponent>(TEXT("WaterableComponent"));
	mPlantMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlantMeshComponent"));
}
void APlantBase::BeginPlay()
{
	mWaterableComponent->mPlant = this;
	mPlantMesh->SetStaticMesh(mPlantStageMeshes[mPlantStageIndex]);
}
void APlantBase::PickedUp()
{

}
void APlantBase::Grow(float rate)
{
	if (mPlantStageIndex >= mPlantStages.Num()) {
		//Stop growing
		return;
	}
	else {
		//Do growth
		//base growth rate times the percentage of water the plant has
		mPlantStages[mPlantStageIndex].mCurrentGrowth += mBaseGrowthRate * (rate);
		if (mPlantStages[mPlantStageIndex].mCurrentGrowth >= mPlantStages[mPlantStageIndex].mRequiredGrowth) {
			//Next stage
			mPlantStageIndex++;
			mPlantMesh->SetStaticMesh(mPlantStageMeshes[mPlantStageIndex]);
		}
	}
	
}

void APlantBase::GetSoil(ASoilBase* soil) {
	mSoil = soil;
}
void APlantBase::Use1()
{
	//??
	FHitResult hit = GetPlayerCameraGenericHit();
	//Find if it hit a soil container
	if (!hit.GetActor()) {
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, TEXT("SoilBase : No actor hit"));
		return;
	}
	if (hit.GetActor()->IsA(ASoilBase::StaticClass())) {
		ASoilBase* c = Cast<ASoilBase>(hit.GetActor());
		//Drop me from inventory first
		DropMe();
		//Give me to the soil container
		c->GetPlant(this);

	}
}
