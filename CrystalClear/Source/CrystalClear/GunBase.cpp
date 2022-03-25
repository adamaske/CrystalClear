// Fill out your copyright notice in the Description page of Project Settings.


#include "GunBase.h"

void AGunBase::Use1()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("GunBase : Use1")));
}																				   
																				   
void AGunBase::Use2()															   
{																				   
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("GunBase : Use2")));
}