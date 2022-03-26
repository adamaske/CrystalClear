// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"
#include "InventoryPickupComponent.h"
#include "InventoryComponent.h"
#include "PlayerUnit.h"
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
	mPlayer = Cast<APlayerUnit>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!mPlayer) {
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, TEXT("InventoryItem : No player"));
	}
}

// Called every frame
void AInventoryItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AInventoryItem::Used(PlayerUnit* player)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, TEXT("Used item"));
}

void AInventoryItem::SetItemState(ItemState state)
{
	mItemState = state;
	UpdateItemState();
}

void AInventoryItem::UpdateItemState()
{
	if (mItemState == ItemState::InWorld) {
		//Physics active
		mesh->SetSimulatePhysics(true);
		mesh->SetEnableGravity(true);
		SetActorEnableCollision(true);
		//Show everyting
		SetActorHiddenInGame(false);
		SetActorTickEnabled(true);
	}
	else if (mItemState == ItemState::InInventory) {
		//Physics active
		mesh->SetSimulatePhysics(false);
		mesh->SetEnableGravity  (false);
		SetActorEnableCollision (false);
		//Show everyting
		SetActorHiddenInGame(true);
		//Tick can run while in inventory
		SetActorTickEnabled(true);
	}
	else if (mItemState == ItemState::InPlayerHand) {
		//Physics disabled
		mesh->SetSimulatePhysics(false);
		mesh->SetEnableGravity(false);
		SetActorEnableCollision(false);
		//Show everyting
		SetActorHiddenInGame(false);
		SetActorTickEnabled(true);
	}
	else if (mItemState == ItemState::Other) {
		//Custom for overrides
	}
}

void AInventoryItem::PickedUp()
{
}


void AInventoryItem::Use1()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("InventoryItem : Used 1"));
}

void AInventoryItem::Use2()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("InventoryItem : Used 2"));
}

FHitResult AInventoryItem::GetPlayerCameraGenericHit()
{
	if (mPlayer) {
		return mPlayer->GetCameraHit();
	}
	return FHitResult();
}

void AInventoryItem::DropMe() {
	mPlayer->inventory->DropItem(this);
}