// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunBase.h"
#include "WaterGun.generated.h"

/**
 * 
 */
UCLASS()
class CRYSTALCLEAR_API AWaterGun : public AGunBase
{
	GENERATED_BODY()
public:

	virtual void Use1() override;
	virtual void Use2() override;

};
