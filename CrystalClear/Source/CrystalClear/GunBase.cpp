// Fill out your copyright notice in the Description page of Project Settings.


#include "GunBase.h"
#include "PlayerUnit.h"
void AGunBase::BeginPlay() {
	Super::BeginPlay();
	if (!mPlayer) {
		mPlayer = Cast<APlayerUnit>(GetWorld()->GetFirstPlayerController()->GetPawn());
	}

	
	TraceParams.AddIgnoredActor(this);
}
void AGunBase::Use1()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("GunBase : Use1")));
}																				   
																				   
void AGunBase::Use2()															   
{																				   
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("GunBase : Use2")));
}

FHitResult AGunBase::GenericTraceShootFunction(float shootDistance)
{
	CameraLoc = mPlayer->FPSCamera->GetComponentLocation();
	CameraRot = mPlayer->FPSCamera->GetComponentRotation();

	End = CameraLoc + (CameraRot.Vector() * shootDistance);
	TraceParams.AddIgnoredActor(this);
	if (GetWorld()->LineTraceSingleByChannel(ShootHitResult, CameraLoc, End, ECC_Visibility)) {
		//If the is not equals the currentWaterable
		return ShootHitResult;

	}
	return ShootHitResult;
}
