// Fill out your copyright notice in the Description page of Project Settings.


#include "PictureLabel.h"

#include "InteractWidget.h"
#include "Components/BoxComponent.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

// Sets default values
APictureLabel::APictureLabel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);

	BoxComp=CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComponent"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetCollisionProfileName(FName("Clue"));

	StaticMeshComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComp->SetupAttachment(BoxComp);
}

// Called when the game starts or when spawned
void APictureLabel::BeginPlay()
{
	Super::BeginPlay();
	TestPC = GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	if (TestPC)
	{
		MainCharacter =TestPC->GetPawn<ATestCharacter>();
		PlayerHUD =TestPC->GetHUD<APlayerHUD>();
	}
	GI = GetGameInstance<UMyTestGameInstance>();
	
	if(M_Overlay)
	{
		StaticMeshComp->SetOverlayMaterial(M_Overlay);
	}
}

void APictureLabel::IInteract()
{
	LookAt();
}

void APictureLabel::LookAt()
{
	MainCharacter->SetPlayerState(EPlayerState::UI);
	FName PictureLabel_FName = FName(*FString::FromInt(PictureLabel_ID));
	FPictureLabelData* PictureLabelData= GI->DT_PictureLabel->FindRow<FPictureLabelData>(PictureLabel_FName , TEXT(""));
	if (PictureLabelData)
	{
		FString Title =PictureLabelData->Title;
		FString Description =PictureLabelData->Description;
		PlayerHUD->InteractUI->SetText_PictureLabel(Title,Description);
	}
	PlayerHUD->InteractUI->SetVisibleCanvasPanel_PictureLabel(true);
}

