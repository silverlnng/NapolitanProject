// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Security_AnimInstance.h"

#include "NPC_Security.h"

void UNPC_Security_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	NPCSecurity=Cast<ANPC_Security>(TryGetPawnOwner());
}
