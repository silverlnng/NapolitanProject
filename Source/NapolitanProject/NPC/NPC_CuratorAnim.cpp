// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_CuratorAnim.h"

void UNPC_CuratorAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(!Curator)
	{
		auto ownerPawn = TryGetPawnOwner();
		Curator = Cast<AChaseStatue>(ownerPawn);
	}
}

void UNPC_CuratorAnim::PlayWalkMontage()
{
	if (!Montage_IsPlaying(WalkMontage)) {
		Montage_Play(WalkMontage);
	}
}
