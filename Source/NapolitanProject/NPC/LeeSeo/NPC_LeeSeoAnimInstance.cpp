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

void UNPC_LeeSeoAnimInstance::PlayJumpSkareMontage2()
{
	if (!Montage_IsPlaying(LeeSeo_JumpSkareMontage2))
	{
		Montage_Play(LeeSeo_JumpSkareMontage2); //몽타주 플레이
	}
}

void UNPC_LeeSeoAnimInstance::PlayJumpSkareMontage3()
{
	if (!Montage_IsPlaying(LeeSeo_JumpSkareMontage3))
	{
		Montage_Play(LeeSeo_JumpSkareMontage3); //몽타주 플레이
	}
}

void UNPC_LeeSeoAnimInstance::AnimNotify_SwitchCamera()
{
	//카메라 바꾸는 노티파이
	LeeSeo->SwitchToMonsterCamera();
}

void UNPC_LeeSeoAnimInstance::AnimNotify_KnifeSound()
{
	LeeSeo->KnifeSound(); //칼로 찌르는 소리 호출
}
