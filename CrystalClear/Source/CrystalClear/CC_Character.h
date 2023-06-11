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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPerspectiveChanged, TEnumAsByte<EPlayerPerspective>, _ToPerspective);


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
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Perspective, meta = (AllowPrivateAccess = "true"))
		TEnumAsByte<EPlayerPerspective> m_Perspective = FirstPerson;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_ChangePerspectiveAction;

#pragma endregion
private:
	//COMPONENTS
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float m_MovementSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float m_LookSensitivty = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		FVector m_CameraOffset = FVector{ -10, 0, 60 };//Ppsition Camera relative to capsule

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* m_Camera;
	/** Spring Arm */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* m_CameraSpringArm;

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


};
