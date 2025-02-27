// Fill out your copyright notice in the Description page of Project Settings.


#include "DetectiveMapGameModeBase.h"

#include "LevelSequencePlayer.h"
#include "MyTestGameInstance.h"
#include "PlayerHUD.h"
#include "TestCharacter.h"
#include "TestPlayerController.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/Interact/InteractWidget.h"

void ADetectiveMapGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	PC=Cast<ATestPlayerController>(GetWorld()->GetFirstPlayerController());
	PC->SetInputMode(FInputModeGameOnly());
	PC->SetShowMouseCursor(false);
	
	MainCharacter=Cast<ATestCharacter>(PC->GetPawn());
	// Q,R 키를 막기. 
	MainCharacter->b_IA_Note_Allowed=false;
	MainCharacter->b_IA_Inven_Allowed=false;
	
	PlayerHUD =PC->GetHUD<APlayerHUD>();

	FTimerHandle GITimer;

	GetWorld()->GetTimerManager().SetTimer(GITimer,[this]()
	{
		PlayerHUD->InteractUI->Border_Inven->SetVisibility(ESlateVisibility::Hidden);
		PlayerHUD->InteractUI->SetVisibleHBox(false);
		PlayerHUD->InteractUI->SetVisibleCrossHair(false);
	
	},0.2f,false);

	// state 에 따라서 구분되도록 하기

	// 스폰되는 게 달라야함 .
	
	GI =GetGameInstance<UMyTestGameInstance>();
	if (GI)
	{
		DetectiveMapState=GI->DetectiveMapState;
	}
	
	switch (DetectiveMapState)
	{
	case EDetectiveMapState::FisrtStart:
		GetWorld()->GetTimerManager().SetTimer(UITimer , [this]()
		{
			PlayerHUD->InteractUI->SetVisibleHBox(true);
			PlayerHUD->InteractUI->SetVisibleCrossHair(true);
		} , 1.f , false);
		break;
	case EDetectiveMapState::FirstEnding:
		
	// 1차 엔딩 시퀀스 작동되도록
		if (FirstEndingSequence)
		{
			ALevelSequenceActor* outActor;
			ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
				GetWorld() , FirstEndingSequence , FMovieSceneSequencePlaybackSettings() , outActor);
			if (SequencePlayer)
			{
				SequencePlayer->OnFinished.AddDynamic(this, &ADetectiveMapGameModeBase::OpenNextLevel);
				SequencePlayer->Play();
			}
		}
		// 시퀀스 다끝나면 맨처음 스타트 화면으로 넘어가도록 하기
		
		break;
	case EDetectiveMapState::SecondEnding:
		
		// 1차 엔딩 시퀀스 작동되도록
		if(SecondEndingSequence)
		{
			ALevelSequenceActor* outActor;
			ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), SecondEndingSequence, FMovieSceneSequencePlaybackSettings(), outActor);
			if(SequencePlayer)
			{
				// 시퀀스 종료 후 LoadNextLevel 호출
				SequencePlayer->OnFinished.AddDynamic(this, &ADetectiveMapGameModeBase::OpenNextLevel);
				SequencePlayer->Play();
			}
		}
		break;
	}
}

void ADetectiveMapGameModeBase::OpenNextLevel()
{
	
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(),StartLevel,true);
		UE_LOG(LogTemp,Warning,TEXT("DetectiveMap:: OpenLevel"))
}
