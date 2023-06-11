// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_Character.h"

//Components
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

//Input
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
// Sets default values
ACC_Character::ACC_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

// Set size for collision capsule
GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

// Create a SpringArm
m_CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
m_CameraSpringArm->SetupAttachment(GetRootComponent());
m_CameraSpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 75.0f));
m_CameraSpringArm->ProbeSize = 10.0;
m_CameraSpringArm->bUsePawnControlRotation = true;
m_CameraSpringArm->bEnableCameraLag = true;

// Create a CameraComponent	
m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
m_Camera->SetupAttachment(GetCapsuleComponent());
m_Camera->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
m_Camera->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void ACC_Character::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(m_DefaultMappingContext, 0);

		}
	}
	
	HandlePerspective(m_Perspective);
}

// Called every frame
void ACC_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACC_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(m_JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(m_JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(m_MoveAction, ETriggerEvent::Triggered, this, &ACC_Character::Move);

		////Looking
		EnhancedInputComponent->BindAction(m_LookAction, ETriggerEvent::Triggered, this, &ACC_Character::Look);

		//Change between third and first person 
		EnhancedInputComponent->BindAction(m_ChangePerspectiveAction, ETriggerEvent::Triggered, this, &ACC_Character::ChangePerspective);

	}
}



void ACC_Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	MovementVector *= m_MovementSpeed * GetWorld()->GetDeltaSeconds();
	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ACC_Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	LookAxisVector *= m_LookSensitivty * GetWorld()->GetDeltaSeconds();
	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACC_Character::ChangePerspective()
{
	m_Perspective = m_Perspective == FirstPerson ? ThirdPerson : FirstPerson;
	HandlePerspective(m_Perspective);
}

void ACC_Character::HandlePerspective(TEnumAsByte<EPlayerPerspective> perspective)
{
	m_CameraSpringArm->bUsePawnControlRotation = m_Perspective == ThirdPerson;
	m_Camera->bUsePawnControlRotation = m_Perspective == FirstPerson;

	m_OnPerspectiveChanged.Broadcast(perspective);
}
