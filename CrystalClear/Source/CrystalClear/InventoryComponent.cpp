// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "PlayerUnit.h"
#include "InventoryItem.h"
#include "CCPlayerController.h"
#include "PlayerHandComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	BackpackItems.SetNum(BackpackSize);
	for (int i = 0; i < BackpackSize; i++) {
		BackpackItems[i] = nullptr;
	}
	ActionBar.SetNum(ActionBarSize);
	for (int i = 0; i < ActionBarSize; i++) {
		ActionBar[i] = nullptr;
	}
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryComponent::ActivateNextItem(int dir)
{
	if (dir == 0) {
		return;
	}
	//Deactivate old active item
	if (ActionBar[ActionBarIndex] != nullptr) {
		ActionBar[ActionBarIndex]->DisableItem();
	}
	ActionBarIndex += dir;
	if (ActionBarIndex >= ActionBarSize) {
		//If it went over, set it to 0
		ActionBarIndex = 0;
	}
	else if (ActionBarIndex < 0) {
		//If it went under 0, set it to the top
		ActionBarIndex = ActionBar.Num() - 1;
	}
	if (ActionBar[ActionBarIndex] != nullptr){
		SetActiveItem(ActionBar[ActionBarIndex]);
	}
	else {
		//There is no items here, so the player is using hands
		player->EnableHands();
	}
	UpdateActionBar();
	
	GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Cyan, FString::Printf(TEXT("ActiveItemIndex : %i"), ActiveItemIndex), true);
}

bool UInventoryComponent::EquipItem(bool equip)
{
	
	return false;
}

AInventoryItem* UInventoryComponent::ActiveItem()
{
	
	return ActionBar[ActionBarIndex];
}

#pragma region Dropping
void UInventoryComponent::DropItem()
{
	//This is for dropping the currently equipped quicka access item
	if (player->bUsesHands) {
		return;
	}
	if (ActionBar[ActionBarIndex] != nullptr) {
		AInventoryItem* item = ActionBar[ActionBarIndex];
		if (item) {
			//player->PlayerHand->DropItem();
			ActionBar[ActionBarIndex] = nullptr;
			
		}
	}
}

void UInventoryComponent::DropItem(AInventoryItem* item)
{
	for (int i = 0; i < BackpackItems.Num(); i++) {
		if (BackpackItems[i] == item) {
			AInventoryItem* item = BackpackItems[i];
			BackpackItems[i] = nullptr;
			item->ActivateItem();
			item->EnablePhysics();
			item->SetActorRotation(player->GetActorRotation());
			item->SetActorLocation(player->GetActorLocation() + FVector(200, 0, 0));
			return;
		}
	}
}
#pragma endregion

void UInventoryComponent::SetInventory(FInventorySave file)
{
	//Clear backpack
	for (int i = 0; i < BackpackItems.Num(); i++) {
		BackpackItems[i] = nullptr;
	}
	//Go through all the ids, spawn accordingly
	for (int i = 0; i < file.BPItems.Num(); i++) {
		AddItem(file.BPItems[i].ID, file.BPItems[i].index);
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
	AddItem(item);
}

void UInventoryComponent::AddItem(int id, int index)
{
	AInventoryItem* item;
	int itemIndex;
	for (auto i = 0; i < allItems.Num(); i++) {
		if (allItems[i]->GetDefaultObject<AInventoryItem>()->ItemID == id) {
			itemIndex = i;
			break;
		}
	}

	//Spawn the item
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	item = GetWorld()->SpawnActor<AInventoryItem>(allItems[itemIndex], GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation(), SpawnParams);

	//This item should just be in inventory, so disable it
	item->DisableItem();
	BackpackItems[index] = item;
}

void UInventoryComponent::AddItem(AInventoryItem* item)
{
	//Adds item
	for (int i = 0; i < BackpackItems.Num(); i++) {
		if (BackpackItems[i] == nullptr) {
			BackpackItems[i] = item;
			BackpackItems[i]->DisableItem();
			return;
		}
	}
	return;
}

void UInventoryComponent::AddItem(AInventoryItem* item, bool activate)
{
	//Adds item
	for (int i = 0; i < BackpackItems.Num(); i++) {
		if (BackpackItems[i] == nullptr) {
			BackpackItems[i] = item;
			BackpackItems[i]->DisableItem();
			if (activate) {
				AddToActionBarRemoveFromBackpack(i, ActiveItemIndex);
			}
			ACCPlayerController::GetInstance()->UpdateHUD();
			return;
		}
	}
	return;
}

#pragma region Blueprint functions
bool UInventoryComponent::SwapItemsInActionBar(AInventoryItem* item1, int item2)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ActionBarSwap"));
	//Get what index its on
	int item1Index;
	if (item1 != nullptr) {
	
		for (int i = 0; i < ActionBar.Num(); i++) {
			if (ActionBar[i] == item1) {
				item1Index = i;
			}
		}
	}
	
	//If both them has items, swap them
	if (ActionBar[item1Index] != nullptr && ActionBar[item2] != nullptr) {
		AInventoryItem* temp = ActionBar[item1Index];
		ActionBar[item1Index] = ActionBar[item2];
		ActionBar[item2] = temp;
	}
	//If there is no item2, just swap it with
	if (ActionBar[item1Index] != nullptr && ActionBar[item2] == nullptr) {
		ActionBar[item2] = ActionBar[item1Index];
		ActionBar[item1Index] = nullptr;
	}
	UpdateActionBar();
	return false;
}

bool UInventoryComponent::AddToActionBarRemoveFromBackpack(int backpackIndex, int actionBarIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Add to action, remove from backpack"));;
	AInventoryItem* temp;
	temp = ActionBar[actionBarIndex];
	ActionBar[actionBarIndex] = BackpackItems[backpackIndex];
	BackpackItems[backpackIndex] = temp;
	UpdateActionBar();
	return false;
}

bool UInventoryComponent::RemoveFromActionBarAddToBackpack(int actionBarIndex, int backpackIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Remove from action, add to backpack"));
	AInventoryItem* temp;
	temp = BackpackItems[backpackIndex];
	BackpackItems[backpackIndex] = ActionBar[actionBarIndex];
	ActionBar[actionBarIndex] = temp;
	UpdateActionBar();
	return false;
}

bool UInventoryComponent::SwapItemsInBackpack(int b1, int b2)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Swap items in backpack"));
	AInventoryItem* temp;
	temp = BackpackItems[b1];
	BackpackItems[b1] = BackpackItems[b2];
	BackpackItems[b2] = temp;
	UpdateActionBar();
	return true;
}

bool UInventoryComponent::PutItemInBackpack(AInventoryItem* item, int toIndex)
{
	int index;
	for (int i = 0; i < BackpackItems.Num(); i++) {
		if (BackpackItems[i] == item) {
			index = i;
		}
	}
	if (BackpackItems[toIndex] != nullptr) {
		UpdateActionBar();
		return SwapItemsInBackpack(index, toIndex);
	}
	return false;
}
#pragma endregion

void UInventoryComponent::SetActiveItem(AInventoryItem* item)
{
	//Find new ActiveItemIndex
	for (int i = 0; i < ActionBar.Num(); i++) {
		if (ActionBar[i] == item) {
			ActionBarIndex = i;
			GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Cyan, FString::Printf(TEXT("ActiveItemIndex : %i"), i));
		}
	}
	//Give the player hand this item
	//player->PlayerHand->GetItem(ActionBar[ActionBarIndex]);
	
	//Enable hands
	player->DisableHands();
}

void UInventoryComponent::UpdateActionBar() {
	for (int i = 0; i < BackpackItems.Num(); i++) {
		//Disable all items in the backpack
		if (BackpackItems[i] != nullptr) {
			BackpackItems[i]->DisableItem();
		}
	}
	for (int i = 0; i < ActionBar.Num(); i++) {
		if (i != ActionBarIndex) {
			//Disable all items that are not the active tiem index
			if (ActionBar[i] != nullptr) {
				ActionBar[i]->DisableItem();
			}
		}
		else if(i == ActionBarIndex){
			//If this is the actionbarindex, and the object here is valid, actiavte it and disable hands
			if (ActionBar[ActionBarIndex] != nullptr) {
				ActionBar[ActionBarIndex]->ActivateItem();
				player->DisableHands();
			}
		}
	}
}

FInventorySave UInventoryComponent::GetInventorySave() {
	FInventorySave save;
	FInventoryItemSave item;
	for (int i = 0; i < BackpackSize; i++) {
		if (BackpackItems[i] != nullptr) {
			item.ID = BackpackItems[i]->ItemID;
			item.index = i;
			save.BPItems.Add(item);
		}
	}
	for (int i = 0; i < ActionBar.Num(); i++) {
		if (ActionBar[i] != nullptr) {
			item.ID = ActionBar[i]->ItemID;
			item.index = i;
			save.QAItems.Add(item);
		}
	}
	return save;
}