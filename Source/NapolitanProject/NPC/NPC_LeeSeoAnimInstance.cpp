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
	if (!Montage_IsPlaying(LeeSeo_JumpSkareMontage1))
	{
		Montage_Play(LeeSeo_JumpSkareMontage1); //몽타주 플레이
	}
}

void UNPC_LeeSeoAnimInstance::JumpToSection(const FString& SectionName)
{
	Montage_JumpToSection((*SectionName), LeeSeo_JumpSkareMontage1);
	UE_LOG(LogTemp, Display, TEXT("JumpToSection: %s"), *SectionName);
}
