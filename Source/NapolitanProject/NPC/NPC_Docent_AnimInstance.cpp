// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Docent_AnimInstance.h"

#include "NapolitanProject/YJ/Docent_JumpScare.h"

void UNPC_Docent_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Docent_JumpScare=Cast<ADocent_JumpScare>(TryGetPawnOwner());
}
