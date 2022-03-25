// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "GunBase.generated.h"

/**
 * 
 */
UCLASS()
class CRYSTALCLEAR_API AGunBase : public AWeaponBase
{
	GENERATED_BODY()
public:

	virtual void Use1() override;
	virtual void Use2() override;
};
