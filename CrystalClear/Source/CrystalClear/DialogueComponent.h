// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponent.h"
#include "DialogueComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYSTALCLEAR_API UDialogueComponent : public UInteractableComponent
{
	GENERATED_BODY()
public:
	virtual void Interact(APlayerUnit* player) override;
};
