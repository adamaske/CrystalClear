// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "PlayerUnit.h"
#include "InventoryItem.h"
#include "PlayerHandComponent.h"
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
}

void UInventoryComponent::ActivateNextItem(int dir)
{
	if (dir == 0) {
		return;
	}
	int index;
	for (int i = 0; i < playerItems.Num(); i++) {
		if (playerItems[i] == activeItem) {
			index = 0;
		}
	}
	int nextIndex = index;
	if (dir == 1) {
		for (int i = index+1; i < playerItems.Num(); i++) {
			if (playerItems[i]) {
				nextIndex = i;
				break;
			}

		}
		for (int i =0; i > index; i++) {
			if (playerItems[i]) {
				nextIndex = i;
				break;
			}
		}
	}
	else if (dir == -1) {
		for (int i = index-1; i > -1; i--) {
			if (playerItems[i]) {
				nextIndex = i;
				break;
			}
		}
		//Start at index
		for (int i = playerItems.Num(); i > index +1; i++) {
			if (playerItems[i]) {
				nextIndex = i;
				break;
			}
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Swapped item %d"), nextIndex));
	SetActiveItem(playerItems[nextIndex]);
}

void UInventoryComponent::CreateAllItems()
{
}

void UInventoryComponent::InitInventory(APlayerUnit* p)
{

}

void UInventoryComponent::AddItem(AInventoryItem* item, bool setAsActiveItem)
{
	playerItems.Add(item);
	if (setAsActiveItem) {
		DisableItem(activeItem, true);
		activeItem = item;
		SetActiveItem(item);
	}
	else {
		DisableItem(item, true);
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
	DisableItem(item, true);
}

void UInventoryComponent::SetActiveItem(AInventoryItem* item)
{
	if (activeItem) {
		DisableItem(activeItem, true);
	}
	activeItem = item;
	//Activate it
	DisableItem(activeItem, false);
	player->handComponent->GetItem(Cast<AActor>(activeItem));
	
}

void UInventoryComponent::DisableItem(AInventoryItem* item,bool disable) {
	if (!item) {
		return;
	}
	if (disable) {
		item->SetActorHiddenInGame(true);
		item->SetActorTickEnabled(false);
		item->SetActorEnableCollision(false);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Disabled actor"));
	}
	else {
		item->SetActorHiddenInGame(false);
		item->SetActorTickEnabled(true);
		item->SetActorEnableCollision(true);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Enabled actor"));
	}
}