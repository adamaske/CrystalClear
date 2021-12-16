// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryPickupComponent.h"
#include "InventoryItem.h"
void UInventoryPickupComponent::Interact(APlayerUnit* player) {
	//Be picked up
	AInventoryItem* item;
	if (GetOwner()->IsA(AInventoryItem::StaticClass())) {
		item = Cast<AInventoryItem>(GetOwner());
		player->PickupItem(item, bActivateMeOnPickup);
	}
}
void UInventoryPickupComponent::BeginPlay()
{
}

void UInventoryPickupComponent::PickedUp()
{
}

void UInventoryPickupComponent::Dropped()
{
}

