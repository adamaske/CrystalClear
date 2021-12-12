// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CrystalClearGameModeBase.generated.h"

/**
 * 
 */
class APlayerUnit;
UCLASS()
class CRYSTALCLEAR_API ACrystalClearGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ACrystalClearGameModeBase();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APlayerUnit> PlayerBlueprint;

	UPROPERTY(EditAnywhere, Category = "Player")
	class APlayerUnit* PlayerUnit{ nullptr };
};
