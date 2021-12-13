// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveableComponent.h"

void UMoveableComponent::BeginPlay() {

	if (GetOwner()->GetRootComponent()->IsA(UPrimitiveComponent::StaticClass())){
		root = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	}
}
void UMoveableComponent::Interact(APlayerUnit* player) {

}

void UMoveableComponent::StartBeingMoved(APlayerUnit* player) {
	//Set the shiz
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Started being moved"));
	root->SetSimulatePhysics(true);
	if (bDisableGravityWhileGrabbed) {
		root->SetEnableGravity(false);
	}
	if (bDisablePhysicsWhenCarried) {
		root->SetSimulatePhysics(false);
	}
}
void UMoveableComponent::EndBeingMoved() {
	//Set the shiz
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Ended being moved"));
	if (bDisablePhysicsWhenNotCarried) {
		root->SetSimulatePhysics(false);
	}
	if (bDisableGravityWhileGrabbed) {
		root->SetEnableGravity(true);
	}
	if (bDisablePhysicsWhenCarried) {
		root->SetSimulatePhysics(true);
	}
}