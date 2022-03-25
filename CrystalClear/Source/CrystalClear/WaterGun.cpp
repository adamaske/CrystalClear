// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterGun.h"

void AWaterGun::Use1()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("WaterGun : Use1")));
}																				   
																				   
void AWaterGun::Use2()															   
{																				   
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("WaterGun : Use2")));
}