// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"
#include "InventoryPickupComponent.h"
// Sets default values
AInventoryItem::AInventoryItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	pickupComponent = CreateDefaultSubobject<UInventoryPickupComponent>(TEXT("PickupComponent"));

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

}

// Called when the game starts or when spawned
void AInventoryItem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInventoryItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FInventoryItemInfo AInventoryItem::GetItemSave()
{
	FInventoryItemInfo save;
	save.ItemID = ItemID;
	return save;
}

void AInventoryItem::Used(PlayerUnit* player)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, TEXT("Used item"));
}


void AInventoryItem::DisableItem()
{
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	SetActorEnableCollision(false);
}

void AInventoryItem::DisablePhysics()
{
	mesh->SetSimulatePhysics(false);
	mesh->SetEnableGravity(false);
	SetActorEnableCollision(false);
}

void AInventoryItem::EnablePhysics()
{
	mesh->SetSimulatePhysics(true);
	mesh->SetEnableGravity(true);
	SetActorEnableCollision(true);
}

void AInventoryItem::ActivateItem()
{
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
	SetActorEnableCollision(true);
}

void AInventoryItem::Use1()
{
}

void AInventoryItem::Use2()
{
}
