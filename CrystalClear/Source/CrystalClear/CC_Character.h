// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "InputActionValue.h"

#include "CC_Character.generated.h"

class UInputMappingContext;
class UInputComponent;
class UInputAction;

class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class USpringArmComponent;
class UAnimMontage;
class USoundBase;

UENUM(BlueprintType)
enum EPlayerPerspective { FirstPerson, ThirdPerson };
UENUM(BlueprintType)
enum EThirdPersonPerspectiveSide { Right, Left };

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPerspectiveChanged, TEnumAsByte<EPlayerPerspective>, _ToPerspective);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThirdPersonPerspectiveSideChanged, TEnumAsByte<EThirdPersonPerspectiveSide>, _ToSide);



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShoot);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAim, bool, _IsAiming);


UCLASS()
class CRYSTALCLEAR_API ACC_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACC_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Perspective, meta = (AllowPrivateAccess = "true"))
		float m_RotateTowardsCameraForwardSpeed = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Perspective, meta = (AllowPrivateAccess = "true"))
		float m_RotateTowardsMovementDirectionSpeed = 180.f;
public:

#pragma region Perspective
	/* Perspective : first or third person*/
public:
	UFUNCTION(BlueprintCallable)
		void ChangePerspective();

	UPROPERTY(BlueprintAssignable, Category = Perspective)
		FOnPerspectiveChanged m_OnPerspectiveChanged;
	UFUNCTION(BlueprintCallable)
		void HandlePerspective(TEnumAsByte<EPlayerPerspective> perspective);

	//Change the location of the camera boom based on control rotation
	FVector GetCameraBoomLocalLocation();
	FVector GetCameraBoomLocalLocation(float _theta);
	//Change the location of the camera boom based on control rotation
	void HandleCameraBoomLocation();


	//Set the rotatio of the camera boom based on its location
	FRotator GetCameraBoomRotation();
	void HandleCameraBoomRotation();
	//Set the  CameraBoom yaw theta based on the First Person rotation
	void SetYawThetaToCurrentRotation();
	float GetYawRotation();

	//This should be called during 
	void RotateTowardMovement();

	void RotateTowardCameraForward();


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Perspective, meta = (AllowPrivateAccess = "true"))
		TEnumAsByte<EPlayerPerspective> m_Perspective = FirstPerson;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Perspective, meta = (AllowPrivateAccess = "true"))
		TEnumAsByte<EThirdPersonPerspectiveSide> m_PerspectiveSide = Right;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_ChangePerspectiveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Perspective, meta = (AllowPrivateAccess = "true"))
		float m_ThirdPersonCameraRadius = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Perspective, meta = (AllowPrivateAccess = "true"))
		float m_ThirdPersonCameraZOffset = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Perspective, meta = (AllowPrivateAccess = "true"))
		float m_ArmLengthToRadiusRatio = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Perspective, meta = (AllowPrivateAccess = "true"))
		float m_CameraBoomYawTheta = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Perspective, meta = (AllowPrivateAccess = "true"))
		float m_CameraBoomPitchTheta = 0;

#pragma endregion


private:
	//COMPONENTS
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float m_MovementSpeed = 100.f;

#pragma region Camera
private:
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* m_Camera;
	/** Spring Arm */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* m_CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		FVector m_CameraBoomOffset = FVector{ 0, 0, 50 };//

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Perspective, meta = (AllowPrivateAccess = "true"))
		float m_CameraBoomTargetArmLength = 500;
	//Sensitivies
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float m_FirstPersonLookSensitivty = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float m_ThirdPersonLookYawSensitivty = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float m_ThirdPersonLookPitchSensitivty = 10.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		FVector m_CameraOffset = FVector{ -10, 0, 60 };//Ppsition Camera relative to capsule


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float m_ZoomSpeed = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_ZoomAction;

	void Zoom(const FInputActionValue& Value);
#pragma endregion

#pragma region Input
private:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputMappingContext* m_DefaultMappingContext;
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_MoveAction;
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_LookAction;
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_JumpAction;
	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_InteractAction;
	/** Shooting / UseHeld Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_ShootAction;
	/** Aiming Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_AimAction;
#pragma endregion

#pragma region Interaction
public:
	void RequestInteraction();
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float m_InteractionRange = 300.f;
#pragma endregion

#pragma region Combat
public:
	void Shoot();

	void StartAim();
	void Aim();
	void EndAim();
	bool b_Aiming = false;

	UPROPERTY(BlueprintAssignable, Category = Combat, meta = (AllowPrivateAccess = "true"))
		FOnAim m_OnAim;

	UPROPERTY(BlueprintAssignable, Category = Combat, meta = (AllowPrivateAccess = "true"))
		FOnShoot m_OnShoot;
private:
	/** Desrired Pistol Location */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		USceneComponent* m_DesiredPistolLocation;
	/** Temporary arm for aiming */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		USceneComponent* m_TemporaryArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		float m_TemporaryArmMinYaw = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		float m_TemporaryArmMaxYaw = 90.f; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		float m_TemporaryArmMinPitch = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		float m_TemporaryArmMaxPitch = 90.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		float m_AimRotationRate = 10.f;

	void TemporaryArmToIdle();
	
	void TemporaryArmToAimLocation(FVector location);
	float m_TemporaryArmIdleRotationRate = 10.f;
	float m_TemporaryArmIdleYaw = 0;
	float m_TemporaryArmIdlePitch = 0;

#pragma endregion

#pragma region Helpers

	UFUNCTION(BlueprintCallable)
		AActor* GetActorInFronOfCamera();

#pragma endregion

};
