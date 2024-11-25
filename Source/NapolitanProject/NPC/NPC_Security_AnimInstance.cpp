// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Security_AnimInstance.h"

#include "NPC_Security.h"

void UNPC_Security_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	NPCSecurity=Cast<ANPC_Security>(TryGetPawnOwner());
}

void UNPC_Security_AnimInstance::AnimNotify_AttackStart()
{
	if (NPCSecurity)
	{
		NPCSecurity->OnMyAttackStart();
		//UE_LOG(LogTemp , Warning , TEXT("AnimNotify_AttackStart"));
	}
	
}

void UNPC_Security_AnimInstance::AnimNotify_AttackMiddle()
{
	if (NPCSecurity)
	{
		NPCSecurity->OnMyAttackMiddle();
	}
}

void UNPC_Security_AnimInstance::AnimNotify_AttackEnd()
{
	if (NPCSecurity)
	{
		NPCSecurity->OnMyAttackEnd();
	}
}

void UNPC_Security_AnimInstance::playIdleMontage()
{
	if (IdleMontage)
	{
		Montage_Play(IdleMontage);
	}
}
