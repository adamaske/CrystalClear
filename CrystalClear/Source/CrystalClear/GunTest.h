// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "GunTest.generated.h"

/**
 * 
 */
UCLASS()
class CRYSTALCLEAR_API AGunTest : public AInventoryItem
{
	GENERATED_BODY()

	virtual void Use1() override;
	virtual void Use2() override;
};
