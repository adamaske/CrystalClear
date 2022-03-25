// Fill out your copyright notice in the Description page of Project Settings.


#include "GunTest.h"

void AGunTest::Use1()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("GunTest : Shot weapon")));
}


void AGunTest::Use2()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("GunTest : Started aiming")));
}

