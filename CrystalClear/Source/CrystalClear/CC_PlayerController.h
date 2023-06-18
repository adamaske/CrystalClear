// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "InputActionValue.h"

#include "CC_PlayerController.generated.h"

/**
 * 
 */

class UInputMappingContext;
class UInputComponent; 
class UInputAction;

class ACC_Character;
class ACC_HUD;
class ACC_CameraManager;

//Pausing delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPaused);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnpaused);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitGame);

UCLASS()
class CRYSTALCLEAR_API ACC_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable)
	void ChangePause();

	UPROPERTY(BlueprintAssignable, Category = "Game Control")
		FOnPaused m_OnPaused;
	UPROPERTY(BlueprintAssignable, Category = "Game Control")
		FOnUnpaused m_OnUnpaused;

	UFUNCTION(BlueprintCallable)
		void ExitGame();
	UPROPERTY(BlueprintAssignable, Category = "Game Control")
		FOnExitGame m_OnGameExit;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
		ACC_Character* m_Player; 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
		ACC_HUD* m_HUD;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
		ACC_CameraManager* m_CameraManagerBP;
	
	/** MappingContext for Game Control, i.e Pausing, Saving, etc*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputMappingContext* m_ControlMappingContext;
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_PauseAction;


	bool b_Paused = false;
};
