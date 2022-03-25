// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterableComponent.h"
#include "PlantBase.h"
// Sets default values for this component's properties
UWaterableComponent::UWaterableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWaterableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWaterableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWaterableComponent::GetWater(float amount)
{
	if (mWaterLevel > mMaxWaterLevel) {
		mMaxWaterLevel += amount;
		mWaterLevel = FMath::Clamp(mWaterLevel, 0.f, mMaxWaterLevel);
	}
	
}

