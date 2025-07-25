// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundControlActor.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "DialogueUI/NPCDialogueWidget.h"
#include "DialogueUI/NPCResultWidget.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

// Sets default values
ASoundControlActor::ASoundControlActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);
	AudioComp1=CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent1"));
	AudioComp1->SetupAttachment(RootComponent);
	
	AudioComp2=CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent2"));
	AudioComp2->SetupAttachment(RootComponent);

	TextAudioComp2=CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent3"));
	TextAudioComp2->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ASoundControlActor::BeginPlay()
{
	Super::BeginPlay();
	if (LobbyRoom)
	{
		AudioComp1->SetSound(LobbyRoom);
	}
	AudioComp1->Play();
	AudioComp2->Stop();
	
	TextAudioComp2->SetSound(textSound);		
	TextAudioComp2->Stop();
	
	TestPC = GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	if (TestPC)
	{
		MainCharacter =TestPC->GetPawn<ATestCharacter>();
		PlayerHUD =TestPC->GetHUD<APlayerHUD>();
	}

	// 안전한 포인터 사용 (현재 객체를 안전하게 참조)
	TWeakObjectPtr<ASoundControlActor> WeakThis = this;

	FTimerHandle UITimer;

	GetWorld()->GetTimerManager().SetTimer(UITimer,[&, WeakThis]()
	{
		if (PlayerHUD && PlayerHUD->NPCDialogueUI)
		{
			PlayerHUD->NPCDialogueUI->SoundControlActor=this;
			PlayerHUD->NPCResultUI->SoundControlActor=this;
		}
	},1.0f,false);
}

// Called every frame
void ASoundControlActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoundControlActor::BGSoundChange(USoundCue* Sound)
{
	USoundBase* SoundBase_ = Sound;
	bInLobby=false;
	AudioComp1->FadeOut(7.f , 0.f);
	// AudioComp2 에 다른 음원으로 교체한뒤
	AudioComp2->SetSound(SoundBase_);

	AudioComp2->FadeIn(14.f , 1.f);
}

void ASoundControlActor::SoundChangeOriginLobby()
{
	AudioComp2->FadeOut(3.f , 0.f);
	// 2층의 배경음 끄고
	AudioComp1->FadeIn(3.f , 1.f);
	// 다시 1층의 배경음을 키다
	bInLobby = true;
}

void ASoundControlActor::TextSoundChange(bool value)
{
	if (value)
	{
		TextAudioComp2->Play();
	}
	else
	{
		TextAudioComp2->Stop();
	}
}
