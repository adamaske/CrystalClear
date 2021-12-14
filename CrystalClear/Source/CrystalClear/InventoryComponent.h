// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"
USTRUCT()
struct FInventorySave {
	GENERATED_BODY()
public:
	TArray<int> IDs;

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

	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<class AInventoryItem>> allItems;
	UPROPERTY(EditAnywhere)
		TArray<class AInventoryItem*> playerItems;
	UPROPERTY(EditAnywhere)
		class AInventoryItem* activeItem;

	void DisableItem(AInventoryItem* item,bool disable);
	void AddItem(AInventoryItem* item, bool setAsActiveItem);
	void AddItem(int id);
	void SetActiveItem(AInventoryItem* item);
	void SetActiveItem(int index);
	class APlayerUnit* player;
	void ActivateNextItem(int dir);
	int activeItemIndex = 0;
private:
	void CreateAllItems();
	void InitInventory(class APlayerUnit* player);

};
