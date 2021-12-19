// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "PlayerUnit.generated.h"

USTRUCT()
struct FPlayerSave {
	GENERATED_BODY()
public:
		UPROPERTY(VisibleAnywhere, Category = Basic)
		double x = 0;
	UPROPERTY(VisibleAnywhere, Category = Basic)
		double y = 0;
	UPROPERTY(VisibleAnywhere, Category = Basic)
		double z = 0;
};

UCLASS()
class CRYSTALCLEAR_API APlayerUnit : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerUnit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
		float HP = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
		FString playerName = "Adam";
private:

#pragma region Saving & Loading
public:

		FPlayerSave GetPlayerSave();
	class UCCSaveGame* saveGame;
private:
	void SaveGame();
	void LoadGame();
	//Function called when saving finished
	void SaveGameDelegateFunction(const FString& SlotName, const int32 UserIndex, bool bSuccess);
	void LoadGameDelegateFunction(const FString& SlotName, const int32 UserIndex, class USaveGame* LoadedGameData);
	//Set the player
	void SetPlayer(FPlayerSave save);
#pragma endregion


#pragma region Move functions
private:
// Handles input for moving forward and backward.
	UFUNCTION()
		void MoveForward(float Value);
	// Handles input for moving right and left.
	UFUNCTION()
		void MoveRight(float Value);
	// Sets jump flag when key is pressed.
	UFUNCTION()
		void StartJump();
	// Clears jump flag when key is released.
	UFUNCTION()
		void StopJump();
	//Direction vector
	FVector Direction;
#pragma endregion

#pragma region Camera
public:
	// FPS camera.
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* FPSCamera;
#pragma endregion

#pragma region Inventory & Items
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UInventoryComponent* inventory;
	//
	void PickupItem(class AInventoryItem* item, bool activate);
	//Hand, where items are held
	UPROPERTY(EditAnywhere)
		class USceneComponent* hand;

	//Put away items/Enable hands
	void PutAwayItem();

	//Cycle trough items
	void NextItem(float dir);
	
	//Enable hands, switches modes for player and inventory
	bool bUsesHands = false;
	void EnableHands();
	void DisableHands();

private:
	//Using Items
	void UseLeftClick();
	void UseRightClick();
	//Drop item
	void DropItem();
	//Hands
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* handsMesh;

#pragma endregion

#pragma region Interaction
public:
	UPROPERTY(EditAnywhere)
		float InteractionDistance = 200;
private:
	//Interacting
	UFUNCTION()
		void Interact();
	class UInteractableComponent* interactable;
#pragma endregion

#pragma region Moving Items
public:

	UPROPERTY(EditAnywhere)
		class USceneComponent* GrabLocation;
	UPROPERTY(EditAnywhere)
		float MoveableDistance = 200;
private:
	//Carrying items

	//Moving
	void StartMovingItem();
	void EndMovingItem();
	class UMoveableHandler* moveableHandler;

#pragma endregion
};
