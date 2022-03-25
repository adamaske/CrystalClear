// Fill out your copyright notice in the Description page of Project Settings.


#include "Bucket.h"
#include "PlantBase.h"
void ABucket::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (mPlant) {

	}
	
}


void ABucket::GetSoil() {

}

void ABucket::GrowPlant()
{
	if (mPlant) {
		mPlant->Grow(1);
	}
}
