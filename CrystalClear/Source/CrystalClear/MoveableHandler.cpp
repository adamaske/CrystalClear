// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveableHandler.h"
#include "MoveableComponent.h"

// Sets default values for this component's properties
UMoveableHandler::UMoveableHandler()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMoveableHandler::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMoveableHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsMoving) {

		moveable->GetOwner()->SetActorLocation(player->GrabLocation->GetComponentLocation());
	}
}

void UMoveableHandler::StartMoving(UMoveableComponent* comp, FVector offset)
{
	moveable = comp;
	moveable->StartBeingMoved(player);
	bIsMoving = true;
	LocationOffsetMoveableItem = offset;
}

void UMoveableHandler::EndMoving()
{
	if (bIsMoving) {
		bIsMoving = false;
		if (moveable) {
			moveable->EndBeingMoved();
			moveable = nullptr;
		}
	}
}

