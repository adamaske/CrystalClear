// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CC_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CRYSTALCLEAR_API UCC_GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	//Init
	virtual void Init() override;

	virtual void Shutdown() override;

	//Not sure what this does
	virtual void OnStart() override;
};
