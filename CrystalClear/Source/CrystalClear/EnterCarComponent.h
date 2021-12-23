// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponent.h"
#include "EnterCarComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYSTALCLEAR_API UEnterCarComponent : public UInteractableComponent
{
	GENERATED_BODY()
public:
	virtual void Interact(APlayerUnit* player) override;

};
