// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_Character.h"

//Components
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"

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

	m_DesiredPistolLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Desired Pistol Location"));
	m_DesiredPistolLocation->SetupAttachment(GetRootComponent());
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
	if (b_Aiming) {
		Aim();
	}
	if (m_Perspective == ThirdPerson) {
		if (b_Aiming) {
			//RotateTowardCameraForward();//TODO Implement
			
		}
		else {
			//RotateTowardMovement();
		}
	}
	
	HandleCameraBoomLocation();
	HandleCameraBoomRotation();
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
		//EnhancedInputComponent->BindAction(m_AimAction, ETriggerEvent::Triggered, this, &ACC_Character::Aim);
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
		const FRotator Rotation = m_Camera->GetComponentRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		//TODO : In third person mode, use the camera instead

		// Move local forward and backwards in first person mode. Move to control rotation in third person mode
		const FVector Forward = m_Perspective == FirstPerson ? GetActorForwardVector() : ForwardDirection;
		const FVector Right = m_Perspective == FirstPerson ? GetActorRightVector() : RightDirection;

		// add movement 
		AddMovementInput(Forward, MovementVector.Y);
		AddMovementInput(Right, MovementVector.X);

		//
	}

	if (m_Perspective == ThirdPerson) {

		//Only do this rotation upon movement
		if (!b_Aiming) {
			RotateTowardMovement();
		}

		HandleCameraBoomLocation();
		HandleCameraBoomRotation();

	}
}

void ACC_Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	float X = LookAxisVector.X;
	float Y = LookAxisVector.Y;

	float DeltaSeconds = GetWorld()->GetDeltaSeconds();
	if (Controller != nullptr)
	{
		//Scale by sens and delta time
		X *= m_Perspective == FirstPerson ? m_FirstPersonLookSensitivty : m_ThirdPersonLookYawSensitivty;
		Y *= m_Perspective == FirstPerson ? m_FirstPersonLookSensitivty : m_ThirdPersonLookPitchSensitivty;
		X *= DeltaSeconds;
		Y *= DeltaSeconds;

		// add yaw and pitch input to controller
		AddControllerYawInput(X);
		AddControllerPitchInput(Y);
		
		if (m_Perspective == ThirdPerson) {
			//add to theta
			m_CameraBoomYawTheta += LookAxisVector.X * m_ThirdPersonLookYawSensitivty * DeltaSeconds;
			m_CameraBoomPitchTheta += LookAxisVector.Y * m_ThirdPersonLookPitchSensitivty * DeltaSeconds;;
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
	//Spring Arm should use pawn rotation in third person mode
	m_CameraBoom->bUsePawnControlRotation = false;// m_Perspective == ThirdPerson;
	
	//Camera should use rotation in FirstPerson mode
	m_Camera->bUsePawnControlRotation = m_Perspective == FirstPerson;
	
	//Attach camera to Root or Boom 
	m_Camera->SetupAttachment(m_Perspective == FirstPerson ? RootComponent : m_CameraBoom);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = m_Perspective == FirstPerson;
	bUseControllerRotationRoll = false;

	if (m_Perspective == ThirdPerson) {
		//TODO : Find what theta the player is facing ? 
		SetYawThetaToCurrentRotation();
		
		HandleCameraBoomLocation();
		HandleCameraBoomRotation();
	}
	
	m_OnPerspectiveChanged.Broadcast(perspective);
}

void ACC_Character::HandleCameraBoomLocation()
{
	//Clamp theta to 0 and 2 PI
	float theta = m_CameraBoomYawTheta;
	auto limit = 2 * PI;
	
	theta = theta > limit ? theta - limit : theta;
	theta = theta < 0.0000f ? limit + theta : theta;

	FVector newLoc = FVector(cos(theta) * m_ThirdPersonCameraRadius, 
							 sin(theta) * m_ThirdPersonCameraRadius, 
							 m_ThirdPersonCameraZOffset);

	m_CameraBoom->SetWorldLocation(GetActorLocation() + newLoc);
}

void ACC_Character::HandleCameraBoomRotation()
{
	//Set Camera Boom length and raidus
	m_CameraBoom->TargetArmLength = m_CameraBoomTargetArmLength;
	m_ThirdPersonCameraRadius = m_CameraBoomTargetArmLength * m_ArmLengthToRadiusRatio;
	
	auto base = GetActorLocation();
	auto pos = m_CameraBoom->GetComponentLocation();

	//Ignore Z axis
	pos.Z = 0;
	base.Z = 0;

	//In relation to the global right vector
	auto a = FVector(0, 1, 0);
	auto b = pos - base;
	//Normalize 
	b.Normalize();

	//Find what side were on by crossing with the global forward axis
	auto dot = FVector::DotProduct(FVector(1, 0, 0), b);
	bool sign = dot >= 0 ? true : false;

	//Get theta between global Rigth crossed with localized boom position
	auto yaw_theta = FMath::Acos(FVector::DotProduct(a, b) / (a.Length() * b.Length()));
	auto yaw_degrees = FMath::RadiansToDegrees(yaw_theta);
	yaw_degrees *= sign ? -1 : 1;

	//Pitch 
	m_CameraBoomPitchTheta = FMath::Clamp(m_CameraBoomPitchTheta, (- PI / 2.f) + 0.01f, (PI / 2.f) - 0.01f);
	float pitch_theta = m_CameraBoomPitchTheta;
	float pitch_degrees = FMath::RadiansToDegrees(-pitch_theta);


	if (m_Perspective != ThirdPerson) {
		return;
	}

	//Set it directly if we're not using pawn control rotation
	//TODO : Should this always be the case ? We need finer control of the rotation than the hidden control rotation gives
	if (!m_CameraBoom->bUsePawnControlRotation) {
		auto Rotation = m_CameraBoom->GetComponentRotation();
		Rotation.Yaw = yaw_degrees;
		Rotation.Pitch = pitch_degrees;
		m_CameraBoom->SetWorldRotation(Rotation);
	}
	//
	//TODO : Fix this, this needs to be set because of the movement function
	if (auto controller = GetController()) {
		auto control = controller->GetControlRotation();
		control.Yaw = yaw_degrees;
		controller->SetControlRotation(control);
		//TODO : this may interfere with first person controls when using control rotation ? 
	}
	//Apply to control rotation
	if (m_CameraBoom->bUsePawnControlRotation) {
		if (auto controller = GetController()) {
			auto control = controller->GetControlRotation();
			control.Yaw = yaw_degrees;
			controller->SetControlRotation(control);
			//TODO : this may interfere with first person controls when using control rotation ? 
		}
	}
}

void ACC_Character::SetYawThetaToCurrentRotation() {

	auto a = GetActorForwardVector();
	auto b = FVector(0, 1, 0);

	auto theta = FMath::Acos(FVector::DotProduct(a, b) / (a.Length() * b.Length()));
	auto degrees = FMath::RadiansToDegrees(theta);

	auto dot = FVector::DotProduct(FVector(1, 0, 0), a);
	auto sign = dot >= 0 ? true : false;

	m_CameraBoomYawTheta = sign ? PI - theta : theta + PI;// -PI;
}

void ACC_Character::RotateTowardMovement()
{
	// TODO : Find what direction we're walking, rotate towards it
	float DeltaSeconds = GetWorld()->GetDeltaSeconds();

	auto forward = FVector(1, 0, 0);
	auto right = FVector(0, 1, 0);

	auto velocity = GetCharacterMovement()->Velocity;
	velocity.Z = 0;
	velocity.Normalize();

	auto theta = FMath::Acos(FVector::DotProduct(forward, velocity) / (forward.Length() * velocity.Length()));
	auto sign = FVector::DotProduct(right, velocity) >= 0 ? true : false;

	float degrees = FMath::RadiansToDegrees(theta);
	degrees *= sign ? 1 : -1;

	const FRotator Rotation = GetActorRotation();
	const FRotator PlayerYawRotation(0, Rotation.Yaw, 0);
	const FRotator DesiredYawRotation(0, degrees, 0);

	float DeltaYaw = PlayerYawRotation.Yaw - DesiredYawRotation.Yaw;

	FRotator LerpedRotation = FMath::RInterpTo(PlayerYawRotation, DesiredYawRotation, DeltaSeconds, m_RotateTowardsMovementDirectionSpeed);
	FRotator NewRotation(Rotation.Roll, LerpedRotation.Yaw, Rotation.Pitch);

	SetActorRotation(NewRotation);
}

void ACC_Character::RotateTowardCameraForward() {
	float DeltaSeconds = GetWorld()->GetDeltaSeconds();
	float RotationRate = 1.f;

	const FRotator Rotation = GetActorRotation();
	const FRotator PlayerYawRotation(0, Rotation.Yaw, 0);

	const FRotator CameraRotation = m_Camera->GetComponentRotation();
	const FRotator CameraYawRotation(0, CameraRotation.Yaw, 0);

	FRotator LerpedRotation = FMath::RInterpTo(PlayerYawRotation, CameraYawRotation, DeltaSeconds, m_RotateTowardsCameraForwardSpeed);
	FRotator NewRotation(Rotation.Roll, LerpedRotation.Yaw, Rotation.Pitch);

	SetActorRotation(NewRotation);
}

void ACC_Character::Zoom(const FInputActionValue& Value) {

	//Increase POV in FirstPerson
	if (m_Perspective == FirstPerson) {
		//TODO : Increase fov
		float direction = Value.Get<float>();
	}

	if (m_Perspective == ThirdPerson) {
		float direction = Value.Get<float>();

		m_CameraBoomTargetArmLength += m_ZoomSpeed * direction * GetWorld()->GetDeltaSeconds();

		m_CameraBoom->TargetArmLength = m_CameraBoomTargetArmLength;
		m_ThirdPersonCameraRadius = m_CameraBoomTargetArmLength * m_ArmLengthToRadiusRatio;
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

	m_OnShoot.Broadcast();
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
	auto range = 100000.f;
	auto end = start + (m_Camera->GetForwardVector() * range);


	FHitResult Hit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(Hit, start, end, ECC_Visibility, params);

	FVector AimLocation(0, 0, 0);
	//TODO : If we hit something ? 
	if (Hit.bBlockingHit && IsValid(Hit.GetActor())) {
		
		AimLocation = Hit.Location;
	}
	else {
		AimLocation = end;
	}

	TemporaryArmToAimLocation(AimLocation);

	//TODO : Calulate how the IK point should move to aim at this

	//TODO : Max values
	auto Rotaton = GetActorRotation();

	FVector Forward = GetActorForwardVector();
	FVector Right = GetActorRightVector();
	FVector Up = GetActorUpVector();

	auto target = AimLocation;
	auto base = GetActorLocation();

	//Direction towards AimLocation
	FVector AimDirection = target - base;
	AimDirection.Normalize();


	//THETA
	float Theta = FMath::Acos(FVector::DotProduct(Up, AimDirection) / (Up.Length(), AimDirection.Length()));
	float ThetaDegrees = FMath::RadiansToDegrees(Theta); 

	//Below : Above
	bool ThetaSign = FVector::DotProduct(-Up, AimDirection) >= 0 ? true : false;//

	//PHI
	AimDirection.Z = 0;
	float Phi = FMath::Acos(FVector::DotProduct(Forward, AimDirection) / (Forward.Length() * AimDirection.Length()));
	float PhiDegrees = FMath::RadiansToDegrees(Phi);

	//Left : Right
	bool PhiSign = FVector::DotProduct(Right, AimDirection) >= 0 ? true : false;
	
	float s = FMath::DegreesToRadians(PhiSign ? PhiDegrees : -PhiDegrees);
	float t = FMath::DegreesToRadians(ThetaSign ? ThetaDegrees : ThetaDegrees);


	GEngine->AddOnScreenDebugMessage(230, 5, FColor::Cyan, FString::Printf(TEXT("Theta Degrees : %f"), ThetaDegrees));
	GEngine->AddOnScreenDebugMessage(231, 5, FColor::Cyan, FString::Printf(TEXT("Theta Sign : %d"), ThetaSign));

	GEngine->AddOnScreenDebugMessage(232, 5, FColor::Cyan, FString::Printf(TEXT("Phi Degrees : %f"), PhiDegrees));
	GEngine->AddOnScreenDebugMessage(233, 5, FColor::Cyan, FString::Printf(TEXT("Phi Sign : %d"), PhiSign));

	//How far out is the character holding the gun ? 
	float DesiredArmLength = 100.f;

	//TODO : Point straigth towards it 
	float X = cos(s) * sin(t) * DesiredArmLength;
	float Y = sin(s) * sin(t) * DesiredArmLength;
	float Z = cos(t) * DesiredArmLength;

	FVector NewArmLocation = FVector(X, Y, Z);
	m_DesiredPistolLocation->SetRelativeLocation(NewArmLocation);

	//TODO : Aim the point at the target
}

void ACC_Character::EndAim()
{
	//TODO can we disable aiming ? 

	b_Aiming = false;
	m_OnAim.Broadcast(b_Aiming);
}

void ACC_Character::TemporaryArmToIdle()
{
	auto Rotation = m_TemporaryArm->GetComponentRotation();
	float Yaw = Rotation.Yaw;
	float Pitch = Rotation.Pitch;


}

void ACC_Character::TemporaryArmToAimLocation(FVector location)
{
	float deltaTime = GetWorld()->GetDeltaSeconds();
	float rotationRate = m_AimRotationRate * deltaTime;

	auto Rotation = m_TemporaryArm->GetComponentRotation();

	FVector target = location;
	FVector base = m_TemporaryArm->GetComponentLocation();
	auto forward = m_TemporaryArm->GetForwardVector();
	auto right = m_TemporaryArm->GetRightVector();
	auto up = m_TemporaryArm->GetUpVector();
	//Yaw
	auto a = forward;
	auto b = target - base;
	a.Z = 0;//Ignore Z axis for this
	b.Z = 0;
	b.Normalize();
	 
	//Find what direction to rotate, left or right, based on sign
	auto yawDot = FVector::DotProduct(right, b);

	//Find the degrees we got
	auto yawTheta = FMath::Acos(FVector::DotProduct(a, b) / (a.Length() * b.Length()));
	auto yawDegrees = FMath::RadiansToDegrees(yawTheta);

	float desiredYawDegrees = Rotation.Yaw + (yawDot >= 0 ? yawDegrees : -yawDegrees); // in theory this sets it to 0

	//TODO : Do we need to rotate the body ? 
	//TODO : Is the desired degrees above out side this range ? 
	//Then rotate body to compensate
	//TODO : MAKE IT SMOOTHER - NEEDS PID CONTROLLER LIKE BEHAVIOUR

	//Pitch
	a = forward;
	b = target - base;
	b.Normalize();

	float pitchDot = FVector::DotProduct(up, b);

	auto pitchTheta = FMath::Acos(FVector::DotProduct(a, b) / (a.Length() * b.Length()));
	auto pitchDegrees = FMath::RadiansToDegrees(pitchTheta);
	
	auto pitchDesired = Rotation.Pitch + (pitchDot >= 0 ? pitchDegrees : -pitchDegrees); 
	

	//Clamp both pitch and yaw
	float yaw = Rotation.Yaw;
	float pitch = Rotation.Pitch;
	//Rotation.Yaw = FMath::Clamp(yaw, m_TemporaryArmMinYaw, m_TemporaryArmMaxYaw);
	//Rotation.Pitch = FMath::Clamp(pitch, m_TemporaryArmMinPitch, m_TemporaryArmMaxPitch);

	//Set to the desired degrees
	Rotation.Yaw = desiredYawDegrees;
	Rotation.Pitch = pitchDesired;

	m_TemporaryArm->SetWorldRotation(Rotation);
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
