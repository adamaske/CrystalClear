// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryItem.h"
#include "InventoryComponent.generated.h"
USTRUCT()
struct FInventoryItemSave {
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
	int ID;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	int index;
};

USTRUCT()
struct FInventorySave {
	GENERATED_BODY()
public:
	TArray<FInventoryItemSave> BPItems;
	int qaIndex = 0;
	TArray<FInventoryItemSave> QAItems;
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

	//Adds item, true = equip this item now
	void AddItem(AInventoryItem* item);
	//Adds item, true = equip this item now
	void AddItem(AInventoryItem* item, bool activateNow);
	//-1 down in the array, 1 = up, 
	void ActivateNextItem(int dir);
	//True = equipp, False = dequip
	bool EquipItem(bool equip);
	//Returns the active item
	AInventoryItem* ActiveItem();

	void DropItem();

	void DropItem(AInventoryItem* item);
	FInventorySave GetInventorySave();
	//Sets the inventory to state from save struct
	void SetInventory(FInventorySave save);

	//Backpack
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AInventoryItem*> BackpackItems;
	int BackpackSize = 20;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AInventoryItem*> ActionBar;
	int ActionBarSize = 5;
	int ActionBarIndex = 2;

	UFUNCTION(BlueprintCallable)
	bool SwapItemsInActionBar(AInventoryItem* item1, int item2);
	UFUNCTION(BlueprintCallable)
	bool AddToActionBarRemoveFromBackpack(int backpackIndex, int actionBarIndex);
	UFUNCTION(BlueprintCallable)
	bool RemoveFromActionBarAddToBackpack(int actionBarIndex, int backpackIndex);
	UFUNCTION(BlueprintCallable)
	bool SwapItemsInBackpack(int bp1, int bp2);
	
	UFUNCTION(BlueprintCallable)
		bool PutItemInBackpack(AInventoryItem* item, int toIndex);

	void UpdateActionBar();
private:

	void SetActiveItem(AInventoryItem* item);

	int ActiveItemIndex = 0;

	void AddItem(int id);
	void AddItem(int id, int index);

};
