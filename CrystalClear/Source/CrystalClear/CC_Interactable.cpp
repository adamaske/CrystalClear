// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_Interactable.h"

// Add default functionality here for any ICC_Interactable functions that are not pure virtual.


void ICC_Interactable::Interact_Implementation(AActor* interactor)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, FString::Printf(TEXT("Interact CPP")));
}


