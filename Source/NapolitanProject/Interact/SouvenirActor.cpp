// Fill out your copyright notice in the Description page of Project Settings.


#include "SouvenirActor.h"

#include "InteractWidget.h"
#include "Components/BoxComponent.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/YJ/NoteUI/NoteWidget.h"
#include "NapolitanProject/YJ/NoteUI/SouvenirWidget.h"

// Sets default values
ASouvenirActor::ASouvenirActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);

	BoxComp=CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComponent"));
	BoxComp->SetupAttachment(RootComponent);
	
	StaticMeshComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComp->SetupAttachment(BoxComp);

	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,ECR_Block);
}

// Called when the game starts or when spawned
void ASouvenirActor::BeginPlay()
{
	Super::BeginPlay();

	TestPC = GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	MainCharacter =TestPC->GetPawn<ATestCharacter>();
	PlayerHUD=TestPC->GetHUD<APlayerHUD>();
	GI = GetGameInstance<UMyTestGameInstance>();
}

// Called every frame
void ASouvenirActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 ASouvenirActor::GetSouvenirID()
{
	return this->SouvenirID;
}

FString ASouvenirActor::GetSouvenirName()
{
	return this->SouvenirName;
}

void ASouvenirActor::OnPickup()
{
	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,ECR_Ignore);
	FName SouvenirName_ = FName(*FString::FromInt(GetSouvenirID()));
			
	FSouvenirData* SouvenirData= GI->DT_SouvenirData->FindRow<FSouvenirData>(SouvenirName_ , TEXT(""));
	if (SouvenirData)
	{
		SouvenirData->Had=true;
	}
	PlayerHUD->NoteUI->WBP_Souvenir->curPage=GetSouvenirID();		
	TestPC->SetSouvenirUICurNumber(GetSouvenirID());

	FTimerHandle SouvenirTimer;
	GetWorldTimerManager().SetTimer(SouvenirTimer, [this]()
	{
		
		PlayerHUD->InteractUI->GetSouvenirEvent(GetSouvenirName());
	}, 0.5f, false);

	FTimerHandle SouvenirTimer2;
	GetWorldTimerManager().SetTimer(SouvenirTimer2, [this]()
	{
		this->Destroy();
	}, 2.0, false);
	
}


