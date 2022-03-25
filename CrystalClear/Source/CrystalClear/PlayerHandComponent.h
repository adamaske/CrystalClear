// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerHandComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYSTALCLEAR_API UPlayerHandComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UPlayerHandComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere)
		class APlayerUnit* player;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hand")
		class USceneComponent* hand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hand")
		class UStaticMeshComponent* handsMesh;

	class AInventoryItem* item;
	void GetItem(class AInventoryItem* ac);
	void EnableItem();
	void DisableItem();

	void HideItem();
	void ShowItem();

	void DropItem();
};
