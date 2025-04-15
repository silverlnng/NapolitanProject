// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_LeeSeoAnimInstance.h"

#include "NPC_LeeSeo.h"

void UNPC_LeeSeoAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	LeeSeo = Cast<ANPC_LeeSeo>(TryGetPawnOwner());
	
}

void UNPC_LeeSeoAnimInstance::PlayJumpSkareMontage1()
{
	Montage_Play(LeeSeo_JumpSkareMontage1); //몽타주 플레이
}
