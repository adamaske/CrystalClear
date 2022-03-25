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
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		float mShootDistance = 300.f;
	FVector CameraLoc;//= player->FPSCamera->GetComponentLocation();
	FRotator CameraRot;// = player->FPSCamera->GetComponentRotation();

	FVector Start = CameraLoc;
	// you need to add a uproperty to the header file for a float Pl
	FVector End = CameraLoc + (CameraRot.Vector() * mShootDistance);
	FHitResult ShootHitResult;
	FCollisionQueryParams TraceParams;
	virtual void Use1() override;
	virtual void Use2() override;

	FHitResult GenericTraceShootFunction(float shootDistance);
};
