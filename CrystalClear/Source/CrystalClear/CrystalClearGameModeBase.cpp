// Copyright Epic Games, Inc. All Rights Reserved.


#include "CrystalClearGameModeBase.h"
#include "PlayerUnit.h"
#include "CCPlayerController.h"

ACrystalClearGameModeBase::ACrystalClearGameModeBase() {
	
	
}
void ACrystalClearGameModeBase::BeginPlay()
{
	
}

void ACrystalClearGameModeBase::Tick(float DeltaSeconds)
{

}

void ACrystalClearGameModeBase::SpawnPlayer()
{
	if (PlayerBlueprint) {
		Player = Cast<APlayerUnit>(GetWorld()->SpawnActor(PlayerBlueprint));
		Player->SetActorLocation(FVector(0, 0, 0));
	}
	
}
