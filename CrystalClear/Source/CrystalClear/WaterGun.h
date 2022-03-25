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
	UPROPERTY(VisibleAnywhere)
		class UWaterableComponent* mCurrentWaterable;
	UPROPERTY(EditAnywhere)
		float mWaterDistance = 300.f;
	UPROPERTY(EditAnywhere)
		float mWateringRate = 0.2f;
	virtual void Use1() override;
	virtual void Use2() override;
	
};
