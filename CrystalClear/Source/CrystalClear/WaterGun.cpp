// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterGun.h"
#include "WaterableComponent.h"
#include "PlayerUnit.h"
void AWaterGun::Use1()
{
	FHitResult hit = GetPlayerCameraGenericHit();

	if (hit.GetActor()) {
		if (hit.GetActor()->GetComponentByClass(UWaterableComponent::StaticClass())) {
			mCurrentWaterable = Cast<UWaterableComponent>(hit.GetActor()->GetComponentByClass(UWaterableComponent::StaticClass()));
			mCurrentWaterable->GetWater(mWateringRate);
		}
	}

	return;
	CameraLoc = mPlayer->FPSCamera->GetComponentLocation();
	CameraRot = mPlayer->FPSCamera->GetComponentRotation();

	End = CameraLoc + (CameraRot.Vector() * mWaterDistance);
	TraceParams.AddIgnoredActor(this);
	if (GetWorld()->LineTraceSingleByChannel(ShootHitResult, Start, End, ECC_Visibility)) {
		//If the is not equals the currentWaterable
		if (mCurrentWaterable->GetOwner() != ShootHitResult.GetActor()) {
			if (ShootHitResult.GetActor()->GetComponentByClass(UWaterableComponent::StaticClass())) {
				mCurrentWaterable = Cast<UWaterableComponent>(ShootHitResult.GetActor()->GetComponentByClass(UWaterableComponent::StaticClass()));
				mCurrentWaterable->GetWater(mWateringRate);
			}
		}
		else {
			mCurrentWaterable->GetWater(mWateringRate);
		}
		
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("WaterGun : Use1")));
}																				   
																				   
void AWaterGun::Use2()															   
{																				   
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("WaterGun : Use2")));
}