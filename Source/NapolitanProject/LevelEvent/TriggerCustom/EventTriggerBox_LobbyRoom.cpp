// Fill out your copyright notice in the Description page of Project Settings.


#include "EventTriggerBox_LobbyRoom.h"

#include "EngineUtils.h"
#include "Components/AudioComponent.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/YJ/SoundControlActor.h"

void AEventTriggerBox_LobbyRoom::BeginPlay()
{
	Super::BeginPlay();
	BindBeginOverlap();
	BindEndOverlap();
	for (TActorIterator<ASoundControlActor> It(GetWorld(), ASoundControlActor::StaticClass()); It; ++It)
	{
		SoundControlActor = *It;
	}
}

void AEventTriggerBox_LobbyRoom::BindBeginOverlap()
{
	Super::BindBeginOverlap();
}

void AEventTriggerBox_LobbyRoom::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 배경음 바꾸기 
	if (OtherActor->IsA(ATestCharacter::StaticClass()))
	{
		//  배경음 달라지게 하기
		if (SoundControlActor&&SoundControlActor->LobbyRoom)
		{
			SoundControlActor->BGSoundChange(SoundControlActor->LobbyRoom);
		}
	}
}

void AEventTriggerBox_LobbyRoom::BindEndOverlap()
{
	Super::BindEndOverlap();
}

void AEventTriggerBox_LobbyRoom::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 배경음 다시 원래대로
	if (OtherActor->IsA(ATestCharacter::StaticClass()))
	{
		
		//  배경음 달라지게 하기
		if (SoundControlActor)
		{
			SoundControlActor->SoundChangeOriginLobby();
		}
		
	}
	
}
