// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

void AWeaponBase::Use1()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("WeaponBase : Use1")));
}

void AWeaponBase::Use2()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("WeaponBase : Use2")));
}
