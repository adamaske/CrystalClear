// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUnit.h"
#include "InteractableComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "MoveableHandler.h"
#include "InventoryComponent.h"
#include "InventoryItem.h"
#include "MoveableComponent.h"
#include "CCSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerHandComponent.h"
// Sets default values
APlayerUnit::APlayerUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a first person camera component.
	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FPSCamera != nullptr);

	// Attach the camera component to our capsule component.
	FPSCamera->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));

	// Position the camera slightly above the eyes.
	FPSCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));

	// Enable the pawn to control camera rotation.
	FPSCamera->bUsePawnControlRotation = true;

	GrabLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Grab Location"));
	GrabLocation->SetupAttachment(FPSCamera);

	moveableHandler = CreateDefaultSubobject<UMoveableHandler>(TEXT("Moveable Handler"));
	moveableHandler->player = this;

	inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	
	PlayerHand = CreateDefaultSubobject<UPlayerHandComponent>(TEXT("Player Hand"));
	//
	mHand = CreateDefaultSubobject<USceneComponent>(TEXT("Hand"));
	mHand->SetupAttachment(FPSCamera);

	mHandsMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandMesh"));
	mHandsMesh->SetupAttachment(FPSCamera);
}

// Called when the game starts or when spawned
void APlayerUnit::BeginPlay()
{
	Super::BeginPlay();
	moveableHandler->player = this;
	inventory->player = this;
	EnableHands();
	check(GEngine != nullptr) {
		// Display a debug message for five seconds. 
	   // The -1 "Key" value (first argument) indicates that we will never need to update or refresh this message.
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Using PlayerUnit"));
	}
}

// Called every frame
void APlayerUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (inventory->ActionBar[inventory->ActionBarIndex]) {
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Cyan, TEXT("Moved item"));
		inventory->ActionBar[inventory->ActionBarIndex]->SetActorLocation(mHand->GetComponentLocation() + inventory->ActionBar[inventory->ActionBarIndex]->mPosistionOffset);
		inventory->ActionBar[inventory->ActionBarIndex]->SetActorRotation(mHand->GetComponentRotation() + inventory->ActionBar[inventory->ActionBarIndex]->mRotationOffset);
	}
}

// Called to bind functionality to input
void APlayerUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up "movement" bindings.
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerUnit::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerUnit::MoveRight);
	// Set up "look" bindings.
	PlayerInputComponent->BindAxis("Turn", this, &APlayerUnit::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerUnit::AddControllerPitchInput);
	// Set up "action" bindings.
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerUnit::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerUnit::StopJump);
	// Set up "interact" bindings.
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerUnit::Interact);
	// Set up "moving" bindings.
	PlayerInputComponent->BindAction("Carry item", IE_Pressed, this, &APlayerUnit::StartMovingItem);
	PlayerInputComponent->BindAction("Carry item", IE_Released, this, &APlayerUnit::EndMovingItem);
	// Set up "MouseScroll" bindings.
	PlayerInputComponent->BindAxis("MouseScroll", this, &APlayerUnit::NextItem);
	// Set up "items" bindings.
	PlayerInputComponent->BindAction("Put Away Item", IE_Pressed, this, &APlayerUnit::PutAwayItem);
	PlayerInputComponent->BindAction("Use 1", IE_Pressed, this, &APlayerUnit::UseLeftClick);
	PlayerInputComponent->BindAction("Use 2", IE_Released, this, &APlayerUnit::UseRightClick);
	PlayerInputComponent->BindAction("Drop Item", IE_Pressed, this, &APlayerUnit::DropItem);
	//Saving
	PlayerInputComponent->BindAction("Save", IE_Pressed, this, &APlayerUnit::SaveGame);
	PlayerInputComponent->BindAction("Load", IE_Pressed, this, &APlayerUnit::LoadGame);
}

#pragma region Move functions
void APlayerUnit::MoveForward(float Value)
{
	// Find out which way is "forward" and record that the player wants to move that way.
	Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void APlayerUnit::MoveRight(float Value)
{
	// Find out which way is "right" and record that the player wants to move that way.
	Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void APlayerUnit::StartJump()
{
	bPressedJump = true;
}

void APlayerUnit::StopJump()
{
	bPressedJump = false;
}
#pragma endregion

#pragma region Interaction functions
void APlayerUnit::Interact() {
	// get the camera transform
	FVector CameraLoc = FPSCamera->GetComponentLocation();
	FRotator CameraRot = FPSCamera->GetComponentRotation();
	//GetActorEyesViewPoint(CameraLoc, CameraRot);

	FVector Start = CameraLoc;
	// you need to add a uproperty to the header file for a float PlayerInteractionDistance
	FVector End = CameraLoc + (CameraRot.Vector() * InteractionDistance);

	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility)) {
		if (HitResult.GetActor()->GetComponentByClass(UInteractableComponent::StaticClass())) {
			interactable = Cast<UInteractableComponent>(HitResult.GetActor()->GetComponentByClass(UInteractableComponent::StaticClass()));
			interactable->Interact(this);
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Didnt hit actor"));
	};

}
#pragma endregion

#pragma region Moving items functions
void APlayerUnit::StartMovingItem()
{
	// get the camera transform
	FVector CameraLoc;
	FRotator CameraRot;
	GetActorEyesViewPoint(CameraLoc, CameraRot);

	FVector Start = CameraLoc;
	// you need to add a uproperty to the header file for a float PlayerInteractionDistance
	FVector End = CameraLoc + (CameraRot.Vector() * MoveableDistance);

	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility)) {
		if (HitResult.GetActor()->GetComponentByClass(UMoveableComponent::StaticClass())) {

			moveableHandler->StartMoving(Cast<UMoveableComponent>(HitResult.GetActor()->GetComponentByClass(UMoveableComponent::StaticClass())),
				GrabLocation->GetComponentLocation() - HitResult.GetActor()->GetActorLocation());
		}
	}
}

void APlayerUnit::EndMovingItem()
{
	moveableHandler->EndMoving();
}

#pragma endregion

#pragma region Inventory and items
void APlayerUnit::UseLeftClick()
{
	
	if (bUsesHands) {
		
	}
	else {
		if(inventory->ActiveItem() != nullptr)
		inventory->ActiveItem()->Use1();
	}
}

void APlayerUnit::UseRightClick()
{
	if (bUsesHands) {
		
	}
	else {
		if (inventory->ActiveItem() != nullptr)
			inventory->ActiveItem()->Use2();
	}
}

void APlayerUnit::DropItem()
{
	inventory->ActionBar[inventory->ActionBarIndex]->EnablePhysics();
	inventory->ActionBar[inventory->ActionBarIndex]->ActivateItem();
	inventory->DropItem();
	
	EnableHands();
}

void APlayerUnit::PickupItem(AInventoryItem* item, bool activate)
{
	inventory->AddItem(item, activate);
}

void APlayerUnit::PutAwayItem()
{
	if (bUsesHands) {
		DisableHands();
		PlayerHand->ShowItem();
	}
	else {
		//If there was an item to enable
		PlayerHand->HideItem();
		EnableHands();
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("Equipped items : % d"), !bUsesHands));
}

void APlayerUnit::NextItem(float dir)
{
	inventory->ActivateNextItem(dir);
}

void APlayerUnit::EnableHands()
{
	if (inventory->ActionBar[inventory->ActionBarIndex]) {
		inventory->ActionBar[inventory->ActionBarIndex]->DisableItem();
		inventory->ActionBar[inventory->ActionBarIndex]->DisablePhysics();
	}
	//Uses hands now
	bUsesHands = true;
	//Enable hands mesh
	PlayerHand->HideItem();
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Cyan,TEXT("Enabled PlayerHands"));
}

void APlayerUnit::DisableHands()
{
	if (inventory->ActionBar[inventory->ActionBarIndex]) {
		inventory->ActionBar[inventory->ActionBarIndex]->ActivateItem();

	}
	
	PlayerHand->ShowItem();
	bUsesHands = false;
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Cyan, TEXT("Disabled PlayerHands"));
}

#pragma endregion

#pragma region Saving & Loading Functions

void APlayerUnit::SaveGame() {
	if (UCCSaveGame* SaveGameInstance = Cast<UCCSaveGame>(UGameplayStatics::CreateSaveGameObject(UCCSaveGame::StaticClass())))
	{
		// Set up the (optional) delegate.
		FAsyncSaveGameToSlotDelegate SavedDelegate;
		// USomeUObjectClass::SaveGameDelegateFunction is a void function that takes the following parameters: const FString& SlotName, const int32 UserIndex, bool bSuccess
		SavedDelegate.BindUObject(this, &APlayerUnit::SaveGameDelegateFunction);

		SaveGameInstance->playerSave = GetPlayerSave();
		SaveGameInstance->inventorySave = inventory->GetInventorySave();
		
		// Start async save process.
		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, "User", 0, SavedDelegate);
	}
}
void APlayerUnit::LoadGame() {
	// Set up the delegate.
	FAsyncLoadGameFromSlotDelegate LoadedDelegate;
	// USomeUObjectClass::LoadGameDelegateFunction is a void function that takes the following parameters: const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData
	LoadedDelegate.BindUObject(this, &APlayerUnit::LoadGameDelegateFunction);
	UGameplayStatics::AsyncLoadGameFromSlot("User", 0, LoadedDelegate);
}

void APlayerUnit::SaveGameDelegateFunction(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	//Print sucess
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("Saved slot : %s"), *SlotName));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("User index : %d"), UserIndex));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("Success : %d"), bSuccess));
}

void APlayerUnit::LoadGameDelegateFunction(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData)
{
	//Print loading
	if (UCCSaveGame* SaveGameInstance = Cast<UCCSaveGame>(LoadedGameData))
	{
		
		SetPlayer(SaveGameInstance->playerSave);
		inventory->SetInventory(SaveGameInstance->inventorySave);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("Loaded slot : %s"), *SlotName));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("User index : %d"), UserIndex));
	}
	
}
FPlayerSave APlayerUnit::GetPlayerSave() {
	FPlayerSave save;
	save.x = GetActorLocation().X;
	save.y = GetActorLocation().Y;
	save.z = GetActorLocation().Z;
	return save;
}
void APlayerUnit::SetPlayer(FPlayerSave save)
{
	FVector newPos = FVector(save.x, save.y, save.z);

	SetActorLocation(newPos);

}

#pragma endregion
