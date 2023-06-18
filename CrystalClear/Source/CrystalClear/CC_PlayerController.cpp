// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_PlayerController.h"
#include "CC_Character.h"
#include "CC_HUD.h"

#include "CC_CameraManager.h"

/** Input */
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ACC_PlayerController::OnPossess(APawn* InPawn) {

	Super::OnPossess(InPawn);
	
	if (ACC_Character* character = CastChecked<ACC_Character>(InPawn)) {
		m_Player = character;
	}

}

void ACC_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	//Get camera manager


	//Get HUD
	//if (ACC_HUD* hud = CastChecked<ACC_HUD>(GetHUD())) {
	//	m_HUD = hud;
	//}
	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(m_ControlMappingContext, 1);
	}
}

void ACC_PlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}


void ACC_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(m_PauseAction, ETriggerEvent::Triggered, this, &ACC_PlayerController::ChangePause);
		
	}
}

void ACC_PlayerController::ChangePause()
{
	b_Paused = !b_Paused;

	b_Paused ? m_OnPaused.Broadcast() : m_OnUnpaused.Broadcast();
}

void ACC_PlayerController::ExitGame()
{
	m_OnGameExit.Broadcast();
}
