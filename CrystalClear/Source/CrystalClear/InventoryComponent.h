// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"
USTRUCT()
struct FInventorySave {
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<int> IDs;
	UPROPERTY(VisibleAnywhere, Category = Basic)
		int active = 0;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYSTALCLEAR_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	class APlayerUnit* player;
	//All items that the player could possibly have
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<class AInventoryItem>> allItems;
	//The items the player actually have
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<class AInventoryItem*> playerItems{nullptr};
	//The currently equipped item
	UPROPERTY(EditAnywhere)
		class AInventoryItem* activeItem;
	//Adds item, true = equip this item now
	void AddItem(AInventoryItem* item, bool setAsActiveItem);
	//-1 down in the array, 1 = up, 
	void ActivateNextItem(int dir);
	
	//True = equipp, False = dequip
	bool EquipItem(bool equip);

	//Returns the active item
	AInventoryItem* ActiveItem();

	void DropItem();

	FInventorySave GetInventorySave();
	//Sets the inventory to state from save struct
	void SetInventory(FInventorySave save);

	//Backpack

private:

	void SetActiveItem(AInventoryItem* item);

	int ActiveItemIndex = 0;

	void AddItem(int id);

	bool CanSetNextItem();

};
