// Fill out your copyright notice in the Description page of Project Settings.


#include "SoilContainer.h"
#include "SoilBase.h"

void ASoilContainer::BeginPlay() {
	
}
void ASoilContainer::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	
	if (mSoil) {
		mSoil->SetActorLocation(GetActorLocation() + FVector(0, 0, 0));
	}
}

void ASoilContainer::GetSoil(ASoilBase* soil)
{
	if (soil) {
		//
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, TEXT("SoilContainer : Got soil"));
		mSoil = soil;
		//Place my soil at my location + offset
		
	}
}

bool ASoilContainer::GetPlant(APlantBase* plant) {
	//Cant place a plant in a container without soil
	if (mSoil) {
		mSoil->GetPlant(plant);
		return true;
	}
	else {
		//Debug, you need soil in this container
		return false;
	}
}

void ASoilContainer::Use1() {
	//??
	FHitResult hit = GetPlayerCameraGenericHit();
	//Find if it hit a soil container
	if (!hit.GetActor()) {
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, TEXT("SoilContainer : No actor hit"));
		return;
	}
	if (hit.GetActor()) {
		DropMe();

		//Place me on the point hit
		SetActorLocation(hit.Location);
		//SetActorRotation(hit.Normal);
	}
}