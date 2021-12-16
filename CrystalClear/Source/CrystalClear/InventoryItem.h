// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryItem.generated.h"

USTRUCT()
struct FInventoryItemInfo {
	GENERATED_BODY()
public:
	int ItemID = 0;
};

UCLASS()
class CRYSTALCLEAR_API AInventoryItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInventoryItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FInventoryItemInfo GetItemSave();

	virtual void Used(class PlayerUnit* player);
	UPROPERTY(EditAnywhere)
	FInventoryItemInfo ItemInfo;
	UPROPERTY(EditAnywhere)
	int ItemID = 0;
	class UPrimitiveComponent* root;
	void DisableItem();
	void ActivateItem();
	void DisablePhysics();
	void EnablePhysics();
	//U
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* mesh;
	//
	UPROPERTY(EditAnywhere)
		class UInventoryPickupComponent* pickupComponent;
	virtual void Use1();
	virtual void Use2();
};
