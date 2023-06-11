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


//Pausing delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPaused);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnpaused);

UCLASS()
class CRYSTALCLEAR_API ACC_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupInputComponent() override;

	void Pause();

	UPROPERTY(BlueprintAssignable, Category = "Game Control")
		FOnPaused m_OnPaused;
	UPROPERTY(BlueprintAssignable, Category = "Game Control")
		FOnUnpaused m_OnUnpaused;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
		ACC_Character* m_Player; 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
		ACC_HUD* m_HUD;

	/** MappingContext for Game Control, i.e Pausing, Saving, etc*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputMappingContext* m_ControlMappingContext;
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_PauseAction;


	bool b_Paused = false;
};
