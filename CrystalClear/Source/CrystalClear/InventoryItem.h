// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryItem.generated.h"


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

	virtual void Used(class PlayerUnit* player);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int ItemID = 0;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		FText UseActionText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		class UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		FText ItemDisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		FText ItemDescription;

	class UInventoryPickupComponent* pickupComponent;
	class UPrimitiveComponent* root;

	void DisableItem();
	void ActivateItem();
	void DisablePhysics();
	void EnablePhysics();
	virtual void Use1();
	virtual void Use2();
};
