// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponent.h"
#include "MoveableComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYSTALCLEAR_API UMoveableComponent : public UInteractableComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void Interact(APlayerUnit* player) override;

	void StartBeingMoved(APlayerUnit* player);
	void EndBeingMoved();
	class UPrimitiveComponent* root;
	UPROPERTY(EditAnywhere)
		bool bDisablePhysicsWhenNotCarried = true;
	UPROPERTY(EditAnywhere)
		bool bDisablePhysicsWhenCarried = true;
	UPROPERTY(EditAnywhere)
		bool bDisableGravityWhileGrabbed = true;
};
