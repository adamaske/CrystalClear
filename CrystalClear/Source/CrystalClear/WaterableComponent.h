// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WaterableComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYSTALCLEAR_API UWaterableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWaterableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waterable")
		float mWaterLevel = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waterable")
		float mMaxWaterLevel = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waterable")
		class APlantBase* mPlant;

	void GetWater(float amount);

};
