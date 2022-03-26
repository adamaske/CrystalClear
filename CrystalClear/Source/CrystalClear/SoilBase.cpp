// Fill out your copyright notice in the Description page of Project Settings.


#include "SoilBase.h"
#include "PlantBase.h"
#include "SoilContainer.h"
#include "WaterableComponent.h"

ASoilBase::ASoilBase() {
	mWaterableComponent = CreateDefaultSubobject<UWaterableComponent>(TEXT("WaterableComponent"));
}
void ASoilBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	Grow();
}
void ASoilBase::GetPlant(APlantBase* plant)
{
	if (plant) {
		if (!mPlant) {
			mPlant = plant;
		}
		else {
			//Discard my current plant
		}
	}
}
void ASoilBase::GetContainer(ASoilContainer* container)
{
	if (container) {
		mContainer = container;
	}
}
void ASoilBase::PickedUp()
{

}
void ASoilBase::Grow()
{
	if (mContainer && mPlant) {
		mPlant->Grow(mWaterableComponent->mWaterLevel / mWaterableComponent->mMaxWaterLevel);
	}
}

void ASoilBase::Use1()
{
	//??
	FHitResult hit = GetPlayerCameraGenericHit();
	//Find if it hit a soil container
	if (!hit.GetActor()) {
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, TEXT("SoilBase : No actor hit"));
		return;
	}
	if (hit.GetActor()->IsA(ASoilContainer::StaticClass())) {
		ASoilContainer* c = Cast<ASoilContainer>(hit.GetActor());
		//Drop me from inventory first
		DropMe();
		//Give me to the soil container
		c->GetSoil(this);
		
	}
}