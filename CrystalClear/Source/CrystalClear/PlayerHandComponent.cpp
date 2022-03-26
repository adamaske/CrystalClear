// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHandComponent.h"
#include "PlayerUnit.h"
#include "InventoryItem.h"
// Sets default values
UPlayerHandComponent::UPlayerHandComponent(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;
	hand = CreateDefaultSubobject<USceneComponent>(TEXT("Hand"));
	handsMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandMesh"));
	//handsMesh->SetupAttachment(GetOwner()->GetRootComponent());
	///hand->SetupAttachment(GetOwner()->GetRootComponent());
	player = Cast<APlayerUnit>(GetOwner());
	
}

// Called when the game starts or when spawned
void UPlayerHandComponent::BeginPlay()
{
	Super::BeginPlay();
	/*if (player) {
		handsMesh->SetupAttachment(player->FPSCamera);
		hand->SetupAttachment(player->FPSCamera);
	}*/
	//handsMesh->SetupAttachment(player->FPSCamera);
	//hand->SetupAttachment(player->FPSCamera);
	
}

// Called every frame
void UPlayerHandComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (item) {
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Cyan, TEXT("Moved item"));
		item->SetActorLocation(hand->GetComponentLocation() + item->mPosistionOffset);
		item->SetActorRotation(hand->GetComponentRotation() + item->mRotationOffset);
	}
	
}

void UPlayerHandComponent::GetItem(AInventoryItem* ac)
{
	if (!ac) {
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Cyan, TEXT("No item sent"));
		return;
	}

	item = ac;
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Cyan, TEXT("Got item"));
	ac->SetActorLocation(hand->GetComponentLocation());
	ac->SetActorRotation(hand->GetComponentRotation());
	ac->SetActorEnableCollision(false);
}

void UPlayerHandComponent::EnableItem() {
	if(item){
		//item->ActivateItem();
	}
}
void UPlayerHandComponent::DisableItem()
{
	if (item) {
		//item->DisableItem();
		//item->DisablePhysics();
	}
	
}

void UPlayerHandComponent::HideItem()
{
	DisableItem();
	//Show hands

	handsMesh->SetVisibility(true, true);
}

void UPlayerHandComponent::ShowItem()
{
	EnableItem();
	//Hide hands
	handsMesh->SetVisibility(false, true);
}

void UPlayerHandComponent::DropItem() {
	EnableItem();
	//item->EnablePhysics();
	item = nullptr;
}
