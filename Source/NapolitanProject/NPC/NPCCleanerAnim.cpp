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

void UNPCCleanerAnim::playCleaningMontage()
{
	if (!Montage_IsPlaying(CleaningMontage)) {
		Montage_Play(CleaningMontage);
	}
}

void UNPCCleanerAnim::AnimNotify_FinishClean()
{
	Cleaner->SetState(CleanerState::Idle); //움직이는 걸로 변경 
	//Cleaner->MobPointActor->SetActorHiddenInGame(true); //대걸레 안보이게 함
}

void UNPCCleanerAnim::AnimNotify_Decal()
{
	Cleaner->SpawnFootsDecal();
	//UE_LOG(LogTemp, Warning, TEXT("AnimNotify_Footstep called at frame: %f"), GetWorld()->GetTimeSeconds());
}
