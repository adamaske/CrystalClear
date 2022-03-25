// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CCPlayerController.generated.h"

/**
 *
 */

UENUM()
enum Controlled
{
	Player     UMETA(DisplayName = "Player"),
	RV      UMETA(DisplayName = "RV")
};
UCLASS()
class CRYSTALCLEAR_API ACCPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		TEnumAsByte<Controlled> Controlled;
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
		void SpawnHUD();
private:
	virtual void SetupInputComponent() override;
#pragma region Player
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
		class APlayerUnit* PlayerUnit;
	void PossessPlayer();
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
		void OpenInventory();
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
		void UpdateHUD();
	
private:
	void GetPlayer();
	
#pragma endregion

#pragma region RV
public:
	void PossessRV();
#pragma endregion

protected:
	static ACCPlayerController* Instance;
public:
	static ACCPlayerController* GetInstance();
};