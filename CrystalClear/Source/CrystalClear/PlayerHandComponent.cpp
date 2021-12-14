// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHandComponent.h"
#include "PlayerUnit.h"
// Sets default values
UPlayerHandComponent::UPlayerHandComponent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;

	hand = CreateDefaultSubobject<USceneComponent>(TEXT("Hand"));
	
}

// Called when the game starts or when spawned
void UPlayerHandComponent::BeginPlay()
{
	Super::BeginPlay();

		//hand->SetupAttachment(player->FPSCamera);

	
}

// Called every frame
void UPlayerHandComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (item) {
		item->SetActorLocation(hand->GetComponentLocation());
		item->SetActorRotation(hand->GetComponentRotation());
	}
	
}

void UPlayerHandComponent::GetItem(AActor* ac)
{
	if (!ac) {
		return;
	}
	item = ac;
	ac->SetActorLocation(hand->GetComponentLocation());
	ac->SetActorRotation(hand->GetComponentRotation());
	ac->SetActorEnableCollision(false);
}

void UPlayerHandComponent::DisableItem()
{
	item = nullptr;
}

