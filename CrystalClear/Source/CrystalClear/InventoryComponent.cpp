// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "PlayerUnit.h"
#include "InventoryItem.h"
// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Cyan, FString::Printf(TEXT("ActiveItemIndex : %d"), ActiveItemIndex));
	if (playerItems[ActiveItemIndex]) {
		playerItems[ActiveItemIndex]->SetActorLocation(player->hand->GetComponentLocation());
		playerItems[ActiveItemIndex]->SetActorRotation(player->hand->GetComponentRotation());
	}
}

void UInventoryComponent::ActivateNextItem(int dir)
{
	if (dir == 0) {
		return;
	}
	if (playerItems[ActiveItemIndex]) {
		playerItems[ActiveItemIndex]->DisableItem();
	}
	ActiveItemIndex += dir;

	if (ActiveItemIndex >= playerItems.Num()) {
		//If it went over, set it to 0
		ActiveItemIndex = 0;
	}
	else if (ActiveItemIndex < 0) {
		//If it went under 0, set it to the top
		ActiveItemIndex = playerItems.Num() - 1;
	}
	if (playerItems[ActiveItemIndex]) {
		SetActiveItem(playerItems[ActiveItemIndex]);
	}
	else {
		//There is no items here, so the player is using hands
		player->EnableHands();
	}
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Cyan, FString::Printf(TEXT("ActiveItemIndex : %d"), ActiveItemIndex));
}


bool UInventoryComponent::EquipItem(bool equip)
{
	if (equip) {
		//Check if the last activeItem can be used enabled again
		if (playerItems[ActiveItemIndex]) {
			//Activate this
			SetActiveItem(playerItems[ActiveItemIndex]);
			//Return true since it could equip
			return true;
		}
		else if(CanSetNextItem()){ 
			//If there is another item that can be enabled, use it
			//Get the next possible item
			SetActiveItem(playerItems[ActiveItemIndex]);
			return true;
		}
	}
	else {
		if (playerItems[ActiveItemIndex]) {
			//Disable activeItem
			playerItems[ActiveItemIndex]->DisableItem();
			return false;
		}
	}
	return false;
}

AInventoryItem* UInventoryComponent::ActiveItem()
{
	return playerItems[ActiveItemIndex];
}

void UInventoryComponent::DropItem()
{

	if (playerItems[ActiveItemIndex]) {
		AInventoryItem* item = playerItems[ActiveItemIndex];
		if (item) {
			playerItems.Remove(item);
			item->ActivateItem();
			item->EnablePhysics();
			item->SetActorRotation(player->GetActorRotation());
			item->SetActorLocation(player->GetActorLocation() + FVector(200, 0 ,0));
			CanSetNextItem();
			player->EnableHands();
		}
	}
}

void UInventoryComponent::InitInventory(FInventorySave file)
{
	//Create all the items
	//Go through all the ids, spawn accordingly
	for (int i = 0; i < file.IDs.Num(); i++) {
		AddItem(file.IDs[i]);
	}
}

void UInventoryComponent::AddItem(int id)
{
	AInventoryItem* item;
	int index;
	for (auto i = 0; i < allItems.Num(); i++) {
		if (allItems[i]->GetDefaultObject<AInventoryItem>()->ItemID == id) {
			index = i;
			break;
		}
	}

	//Spawn the item
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	item = GetWorld()->SpawnActor<AInventoryItem>(allItems[index], GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation(), SpawnParams);

	//This item should just be in inventory, so disable it
	item->DisableItem();
	AddItem(item, false);
}

bool UInventoryComponent::CanSetNextItem()
{
	//Get the next index where there is an item
	for (int i = ActiveItemIndex + 1; i < playerItems.Num(); i++) {
		if (playerItems[i]) {
			ActiveItemIndex = i;
			return true;
		}
	}
	//Go from the the bttom
	//Get the next 
	for (int i = 0; i < ActiveItemIndex; i++) {
		if (playerItems[i]) {
			ActiveItemIndex = i;
			return true;
		}
	}
	//Go through the whole
	for (int i = 0; i < playerItems.Num(); i++) {
		if (playerItems[i]) {
			ActiveItemIndex = i;
			return true;
		}
	}
	//If it didnt find anything, set it to 0
	ActiveItemIndex = 0;
	return false;
}

void UInventoryComponent::AddItem(AInventoryItem* item, bool setAsActiveItem)
{
	playerItems.Add(item);
	
	
	if (setAsActiveItem) {
		SetActiveItem(item);
	}
	else {
		item->DisableItem();
	}
	
}

void UInventoryComponent::SetActiveItem(AInventoryItem* item)
{
	//Disable previous active
	if (playerItems[ActiveItemIndex]) {
		playerItems[ActiveItemIndex]->DisableItem();
	}
	//Find new ActiveItemIndex
	for (int i = 0; i < playerItems.Num(); i++) {
		if (playerItems[i] == item) {
			ActiveItemIndex = i;
		}
	}
	//Activate it
	playerItems[ActiveItemIndex]->SetActorEnableCollision(false);
	playerItems[ActiveItemIndex]->ActivateItem();
	playerItems[ActiveItemIndex]->DisablePhysics();
	//Enabled an item, so the player is no longer using hands
	player->DisableHands();
}
