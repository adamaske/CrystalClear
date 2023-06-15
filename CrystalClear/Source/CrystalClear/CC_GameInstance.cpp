// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_GameInstance.h"

void UCC_GameInstance::Init()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UGameInstance::Init);
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("GI : Init")));
	
	ReceiveInit();
}

void UCC_GameInstance::Shutdown()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("GI : Shutdown")));
	ReceiveShutdown();
}

void UCC_GameInstance::OnStart()
{
}
