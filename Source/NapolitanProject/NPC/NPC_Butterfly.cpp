// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Butterfly.h"

void ANPC_Butterfly::BeginPlay()
{
	Super::BeginPlay();
}

void ANPC_Butterfly::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

int32 ANPC_Butterfly::GetNPCID()
{
	return NPC_ID;
}

int32 ANPC_Butterfly::GetState()
{
	return Super::GetState();
}

void ANPC_Butterfly::Interact()
{
	Super::Interact();
}
