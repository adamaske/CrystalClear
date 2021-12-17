// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerUnit.h"
#include "InventoryComponent.h"
#include "CCSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class CRYSTALCLEAR_API UCCSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UCCSaveGame();
	UCCSaveGame(FString slotName, uint32 index);
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;
	//Player
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FPlayerSave playerSave;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FInventorySave inventorySave;

};
