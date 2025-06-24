// Fill out your copyright notice in the Description page of Project Settings.


#include "SpectatorAnim.h"

void USpectatorAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(!Spectator)
	{
		auto ownerPawn = TryGetPawnOwner();
		Spectator = Cast<ASpector>(ownerPawn);
	}
	
}
