// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCCleanerAnim.h"

void UNPCCleanerAnim::NativeUpdateAnimation(float DeltaSeconds){
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!Cleaner)
	{
		auto ownerPawn = TryGetPawnOwner();
		Cleaner = Cast<ANPC_Cleaner>(ownerPawn);
	}
	
}