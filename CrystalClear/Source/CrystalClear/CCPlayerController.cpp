// Fill out your copyright notice in the Description page of Project Settings.


#include "CCPlayerController.h"
#include "PlayerUnit.h"
ACCPlayerController* ACCPlayerController::Instance;
void ACCPlayerController::BeginPlay() {
	Instance = this;
	GetPlayer();
	SpawnHUD();
	
}
ACCPlayerController* ACCPlayerController::GetInstance()
{
	return Instance;
}
void ACCPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();
	InputComponent->BindAction("OpenInventory", IE_Pressed, this, &ACCPlayerController::OpenInventory);
}

void ACCPlayerController::GetPlayer()
{
	if (GetPawn()->IsA(APlayerUnit::StaticClass())) {
		PlayerUnit = Cast<APlayerUnit>(GetPawn());
		//Set control state
		Controlled = Controlled::Player;
	}
	else if (true) {
		//Set it to rv
		Controlled = Controlled::RV;
	}
}


void ACCPlayerController::PossessPlayer() {
	if (!PlayerUnit) {
		//Spawn Player
		return;
	}
	Possess(PlayerUnit);
}

void ACCPlayerController::PossessRV() {
	//If !RV spawn rv
}



