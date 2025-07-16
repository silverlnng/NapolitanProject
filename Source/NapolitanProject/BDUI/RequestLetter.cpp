// Fill out your copyright notice in the Description page of Project Settings.


#include "RequestLetter.h"

#include "MovieSceneSequencePlaybackSettings.h"
#include <Runtime/LevelSequence/Public/LevelSequencePlayer.h>
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/SaveGISubsystem.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"

void URequestLetter::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Ticket->OnClicked.AddDynamic(this, &URequestLetter::OnTicketClicked);

	// playerCharacter 초기화
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		playerCharacter = Cast<ATestCharacter>(PC->GetPawn());
		if (!playerCharacter)
		{
			UE_LOG(LogTemp, Error, TEXT("playerCharacter is nullptr. Ensure your pawn is of type ATestCharacter."));
		}
	}
	
	GI=GetGameInstance<UMyTestGameInstance>();

	SaveGI=GI->GetSubsystem<USaveGISubsystem>();
}

void URequestLetter::OnTicketClicked()
{
	//UI안보이게 하기
	SetVisibility(ESlateVisibility::Hidden);

	// 기존의 배경음은 stop 하도록 만들기 

	SaveGI->SetLevelMoveToDoor(false);
	SaveGI->LoadedGame=nullptr;
	
	if(OpeningSequence)
	{
		FVector TargetLocation = FVector(-80, -39.5, 90);
		FRotator TargetRotation = FRotator(0, 79, 0);
		//플레이어의 위치 및 각도 이동 후 시퀀스 재생 (X=-79.995544,Y=-39.562537,Z=90.205725)
		playerCharacter->SetActorLocationAndRotation(TargetLocation,TargetRotation);
		
		ALevelSequenceActor* outActor;
		ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), OpeningSequence, FMovieSceneSequencePlaybackSettings(), outActor);
		if(SequencePlayer)
		{
			SequencePlayer->Play();
			
			// ✅ 2. 비동기 레벨 로딩 시작
			//GI->AsyncLoadLoadLevel(MuseumLevel);
			// start level 경로
			FName PathString = TEXT("/Game/JI/Level/HWM_map.HWM_map");
			GI->PreloadLevel(PathString);
			
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				UGameplayStatics::OpenLevelBySoftObjectPtr(this,MuseumLevel,true); //레벨 변경
			}, 4.82f, false);
		}
		
	}
}

