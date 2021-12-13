// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoveableHandler.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYSTALCLEAR_API UMoveableHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoveableHandler();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere)
		class APlayerUnit* player;

	UPROPERTY(VisibleAnywhere)
		bool bIsMoving = false;

	UPROPERTY(VisibleAnywhere)
		class UMoveableComponent* moveable;

	FVector LocationOffsetMoveableItem;
	void StartMoving(UMoveableComponent* comp, FVector offset);
	void EndMoving();
};
