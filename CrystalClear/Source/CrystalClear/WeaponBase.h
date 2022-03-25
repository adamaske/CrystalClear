// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "WeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class CRYSTALCLEAR_API AWeaponBase : public AInventoryItem
{
	GENERATED_BODY()
	virtual void Use1() override;
	virtual void Use2() override;
};
