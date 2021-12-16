// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueComponent.h"

void UDialogueComponent::Interact(APlayerUnit* player) {
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Cyan, TEXT("Started dialouge"));
}