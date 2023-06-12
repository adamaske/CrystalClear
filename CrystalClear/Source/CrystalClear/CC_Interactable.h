// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CC_Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCC_Interactable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CRYSTALCLEAR_API ICC_Interactable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//Interact
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	void Interact(AActor* OtherActor);
	virtual void Interact_Implementation(AActor* OtherActor);

};
