// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_Character.h"

//Components
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
//Interface
#include "CC_Interactable.h"

//Helpers
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"

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
	m_CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	m_CameraBoom->SetupAttachment(GetRootComponent());
	m_CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 75.0f));
	m_CameraBoom->ProbeSize = 10.0;
	m_CameraBoom->bUsePawnControlRotation = true;
	m_CameraBoom->bEnableCameraLag = true;
	
	// Create a CameraComponent	
	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	m_Camera->SetupAttachment(GetCapsuleComponent());
	m_Camera->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	m_Camera->bUsePawnControlRotation = true;

	m_TemporaryArm = CreateDefaultSubobject<USceneComponent>(TEXT("Temporary Arm"));
	m_TemporaryArm->SetupAttachment(GetRootComponent());
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
	

	if (m_Perspective == ThirdPerson) {
		HandleCameraBoomLocation();
		HandleCameraBoomRotation();

	}

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
		//Change between third and first person 
		EnhancedInputComponent->BindAction(m_InteractAction, ETriggerEvent::Triggered, this, &ACC_Character::RequestInteraction);
		//Zooming
		EnhancedInputComponent->BindAction(m_ZoomAction, ETriggerEvent::Triggered, this, &ACC_Character::Zoom);
		//Shooting
		EnhancedInputComponent->BindAction(m_ShootAction, ETriggerEvent::Triggered, this, &ACC_Character::Shoot);
		//Aiming
		EnhancedInputComponent->BindAction(m_AimAction, ETriggerEvent::Started, this, &ACC_Character::StartAim);
		EnhancedInputComponent->BindAction(m_AimAction, ETriggerEvent::Triggered, this, &ACC_Character::Aim);
		EnhancedInputComponent->BindAction(m_AimAction, ETriggerEvent::Completed, this, &ACC_Character::EndAim);
	}
}



void ACC_Character::Move(const FInputActionValue& Value)
{

	
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	MovementVector *= m_MovementSpeed * GetWorld()->GetDeltaSeconds();
	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		const FVector Forward = m_Perspective == FirstPerson ? GetActorForwardVector() : ForwardDirection;
		const FVector Right = m_Perspective == FirstPerson ? GetActorRightVector() : RightDirection;
		// add movement 
		AddMovementInput(Forward, MovementVector.Y);
		AddMovementInput(Right, MovementVector.X);

		
	}

	if (m_Perspective == ThirdPerson) {
		HandleCameraBoomLocation();
		HandleCameraBoomRotation();

	}
}

void ACC_Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		//Scale by sens and delta time
		LookAxisVector.X *= m_Perspective == FirstPerson ? m_FirstPersonLookSensitivty : m_ThirdPersonLookYawSensitivty;
		LookAxisVector.Y *= m_Perspective == FirstPerson ? m_FirstPersonLookSensitivty : m_ThirdPersonLookPitchSensitivty;
		LookAxisVector *= GetWorld()->GetDeltaSeconds();

		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
		
		if (m_Perspective == ThirdPerson) {
			//add to theta
			m_Theta += LookAxisVector.X;
			HandleCameraBoomLocation();
			HandleCameraBoomRotation();
	
			
			
			//GEngine->AddOnScreenDebugMessage(2, 5, FColor::Cyan, FString::Printf(TEXT("Sign : %d"), sign));
			//GEngine->AddOnScreenDebugMessage(3, 5, FColor::Cyan, FString::Printf(TEXT("Theta : %f"), theta));
			//GEngine->AddOnScreenDebugMessage(4, 5, FColor::Cyan, FString::Printf(TEXT("Degrees : %f"), degrees));
		}
	}
}

void ACC_Character::ChangePerspective()
{
	m_Perspective = m_Perspective == FirstPerson ? ThirdPerson : FirstPerson;
	HandlePerspective(m_Perspective);
}

void ACC_Character::HandlePerspective(TEnumAsByte<EPlayerPerspective> perspective)
{
	
	auto Movement = GetCharacterMovement();
	if (!Movement) {
		return;
	}

	//Orient in Third Person
	Movement->bOrientRotationToMovement = m_Perspective == ThirdPerson;
	//Movement->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	//
	//Movement->JumpZVelocity = 700.f;
	//Movement->AirControl = 0.35f;// add yaw and pitch input to controller
	//Movement->MaxWalkSpeed = 500.f;
	//Movement->MinAnalogWalkSpeed = 20.f;
	//Movement->BrakingDecelerationWalking = 2000.f;

	//Spring Arm should use pawn rotation in third person mode
	m_CameraBoom->bUsePawnControlRotation = m_Perspective == ThirdPerson;
	
	//Camera should use rotation in FirstPerson mode
	m_Camera->bUsePawnControlRotation = m_Perspective == FirstPerson;
	
	//Attach camera to Root or Boom 
	m_Camera->SetupAttachment(m_Perspective == FirstPerson ? RootComponent : m_CameraBoom);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = m_Perspective == FirstPerson;
	bUseControllerRotationRoll = false;

	m_Theta = -PI;

	if (m_Perspective == ThirdPerson) {
		HandleCameraBoomLocation();
		HandleCameraBoomRotation();

	}
	

	m_OnPerspectiveChanged.Broadcast(perspective);
}

void ACC_Character::HandleCameraBoomLocation()
{
	//Clamp theta to 0 and PI
	auto limit = 2 * PI;
	m_Theta = m_Theta > limit ? 0 : m_Theta;
	m_Theta = m_Theta < 0 ? limit : m_Theta;

	FVector newLoc = FVector(cos(m_Theta) * m_ThirdPersonCameraRadius, sin(m_Theta) * m_ThirdPersonCameraRadius, m_ThirdPersonCameraZOffset);// (GetActorForwardVector() * dist);
	m_CameraBoom->SetWorldLocation(GetActorLocation() + newLoc);
}

void ACC_Character::HandleCameraBoomRotation()
{
	//This function handles the Yaw rotation of the camera boom based on its angle from the global axis
	//Get angles between global axis and the spring arm
	//Find the normalized "local" position of the spring arm
	auto base = GetActorLocation();
	auto pos = m_CameraBoom->GetComponentLocation();
	//Dont care about the z axis
	pos.Z = 0;
	base.Z = 0;
	//In relation to the global right vector
	auto a = FVector(0, 1, 0);
	auto b = pos - base;
	b.Normalize();

	//Find what side were on by crossing with the global forward axis
	auto dot = FVector::DotProduct(FVector(1, 0, 0), b);
	bool sign = dot >= 0 ? true : false;

	//Get theta between global Rigth crossed with localized boom position
	auto theta = FMath::Acos(FVector::DotProduct(a, b) / (a.Length() * b.Length()));
	auto degrees = FMath::RadiansToDegrees(theta);
	degrees *= sign ? -1 : 1;

	//Apply to control rotation
	if (auto controller = GetController()) {
		//Get control rotation
		auto control = controller->GetControlRotation();
		//Set the Yaw to the new rotation
		control.Yaw = degrees;

		//TODO : this may interfere with first person controls
		controller->SetControlRotation(control);
	}
	
	m_CameraBoom->TargetArmLength = m_CameraBoomTargetArmLength;
	m_ThirdPersonCameraRadius = m_CameraBoomTargetArmLength * m_ArmLengthToRadiusRatio;

}
void ACC_Character::Zoom(const FInputActionValue& Value) {

	//Increase POV in FirstPerson
	if (m_Perspective == FirstPerson) {
		//TODO : Increase fov
		return;
	}
	//

	if (m_Perspective == ThirdPerson) {
		float direction = Value.Get<float>();

		//TODO

		//what happens when we zoom ? 

		m_CameraBoomTargetArmLength += m_ZoomSpeed * direction * GetWorld()->GetDeltaSeconds();

		HandleCameraBoomLocation();
		HandleCameraBoomRotation();

	}
	
}

void ACC_Character::RequestInteraction()
{
	auto actor = GetActorInFronOfCamera();
	if (!IsValid(actor)) {
		return;
	}

	auto interact = Cast<ICC_Interactable>(actor);
	if (!interact) {
		return;
	}

	interact->Execute_Interact(actor, this);
	
}

void ACC_Character::Shoot()
{
	//TODO can we shoot? Are we holing something that is shootable ? 
}

void ACC_Character::StartAim()
{
	//TODO can we aim now ? 

	b_Aiming = true;
	m_OnAim.Broadcast(b_Aiming);
}

void ACC_Character::Aim()
{
	float deltaTime = GetWorld()->GetDeltaSeconds();
	float rotationRate = m_AimRotationRate * deltaTime;

	//TODO : Find what the camera is looking at 
	auto start = m_Camera->GetComponentLocation();
	auto range = 10000.f;
	auto end = start + (m_Camera->GetForwardVector() * range);


	FHitResult Hit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(Hit, start, end, ECC_Visibility, params);

	//TODO : If we hit something ? 
	if (!Hit.bBlockingHit && !IsValid(Hit.GetActor())) {
		//Dont rotate

		//TODO : Player body parrelel with camera
		return;
	}

	FVector target = Hit.Location;
	FVector base = m_TemporaryArm->GetComponentLocation();

	auto forward = m_TemporaryArm->GetForwardVector();
	auto a = forward;
	auto b = target - base;
	a.Z = 0;
	b.Z = 0;
	b.Normalize();

	//Find what direction to rotate, left or right, based on sign
	auto dot = FVector::DotProduct(m_TemporaryArm->GetRightVector(), b);
	bool sign = dot >= 0 ? true : false;

	//Find the degrees we got
	auto theta = FMath::Acos(FVector::DotProduct(a, b) / (a.Length() * b.Length()));
	auto thetaDegrees = FMath::RadiansToDegrees(theta);

	//Rotate yaw towards it
	auto rotation = m_TemporaryArm->GetComponentRotation();
	float desiredDegrees = rotation.Yaw + (sign ? thetaDegrees : -thetaDegrees); // in theory this sets it to 0
	float deltaDegrees = rotation.Yaw - desiredDegrees;
	if (deltaDegrees < 0) {
		deltaDegrees += 360;//Turn it to positive
	}

	//TODO : Do we need to rotate the body ? 
	
	auto minArmRotation = m_TemporaryArmMinYaw;
	auto maxArmRotation = m_TemporaryArmMaxYaw;
	//TODO : Is the desired degrees above out side this range ? 
	//Then rotate body to compensate

	//TODO : MAKE IT SMOOTHER - NEEDS PID CONTROLLER LIKE BEHAVIOUR
	if (thetaDegrees < 10 && thetaDegrees > -10) {
		//Sets to desired
		rotation.Yaw = desiredDegrees;
	}
	else {
		//Increment rotation towards desired
		rotation.Yaw += sign ? rotationRate : -rotationRate;
	}

	m_TemporaryArm->SetWorldRotation(rotation);

	if (m_Perspective == ThirdPerson) {
		HandleCameraBoomLocation();
		HandleCameraBoomRotation();

		//If we're aiming, set control rotation to camera 
		//if (b_Aiming) {
		//	rotation = GetActorRotation();
		//	rotation.Yaw = m_Camera->GetComponentRotation().Yaw;
		//	SetActorRotation(rotation);
		//}

	}

	GEngine->AddOnScreenDebugMessage(100, 5, FColor::Cyan, FString::Printf(TEXT("Aim : %s"), *Hit.GetActor()->GetName()));
	GEngine->AddOnScreenDebugMessage(101, 5, FColor::Cyan, FString::Printf(TEXT("Theta Degrees: %f"), thetaDegrees));
	GEngine->AddOnScreenDebugMessage(102, 5, FColor::Cyan, FString::Printf(TEXT("Theta Desired: %f"), desiredDegrees));
	GEngine->AddOnScreenDebugMessage(103, 5, FColor::Cyan, FString::Printf(TEXT("Delta Degrees: %f"), deltaDegrees));
	GEngine->AddOnScreenDebugMessage(104, 5, FColor::Cyan, FString::Printf(TEXT("Dot Sign : %d"), sign));

}

void ACC_Character::EndAim()
{
	//TODO can we disable aiming ? 

	b_Aiming = false;
	m_OnAim.Broadcast(b_Aiming);
}

AActor* ACC_Character::GetActorInFronOfCamera()
{
	float ExtraLength = m_Perspective == ThirdPerson ? m_CameraBoom->TargetArmLength : 0.f;
	auto start = m_Camera->GetComponentLocation();
	auto end = start + (m_Camera->GetForwardVector() * (m_InteractionRange + ExtraLength));

	FHitResult Hit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(Hit, start, end, ECC_Visibility, params);

	if (!Hit.bBlockingHit && !IsValid(Hit.GetActor())) {
		return nullptr;
	}

	auto actor = Hit.GetActor();
	return actor;
}
