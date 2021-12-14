// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUnit.h"
#include "InteractableComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "MoveableHandler.h"
#include "InventoryComponent.h"
#include "MoveableComponent.h"
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

	handComponent = CreateDefaultSubobject<UPlayerHandComponent>(TEXT("Player Hand"));
	handComponent->hand->SetupAttachment(FPSCamera);
}

// Called when the game starts or when spawned
void APlayerUnit::BeginPlay()
{
	Super::BeginPlay();
	moveableHandler->player = this;
	inventory->player = this;
	handComponent->player = this;
	check(GEngine != nullptr)

		// Display a debug message for five seconds. 
	   // The -1 "Key" value (first argument) indicates that we will never need to update or refresh this message.
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Using PlayerUnit"));
}

// Called every frame
void APlayerUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//UE_LOG(LogTemp, Warning, TEXT("GrabLocation %s"), *(GrabLocation->GetComponentLocation()).ToString());
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

	// Set up "look" bindings.
	PlayerInputComponent->BindAxis("MouseScroll", this, &APlayerUnit::NextItem);
}

void APlayerUnit::MoveForward(float Value)
{
	// Find out which way is "forward" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void APlayerUnit::MoveRight(float Value)
{
	// Find out which way is "right" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
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
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Hit InteractableComponent"));
			interactable->Interact(this);
		}
		
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Didnt hit actor"));
	};
	
}

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

void APlayerUnit::PickupItem(AInventoryItem* item, bool activate)
{
	inventory->AddItem(item, activate);
}

void APlayerUnit::NextItem(float dir)
{
	inventory->ActivateNextItem(dir);
}

FPlayerSave APlayerUnit::GetPlayerSave() {
	FPlayerSave save;
	save.x = GetActorLocation().X;
	save.y = GetActorLocation().Y;
	save.z = GetActorLocation().Z;
	return save;
}