// Fill out your copyright notice in the Description page of Project Settings.


#include "ClueActor.h"

#include "InteractWidget.h"
#include "../GameFrameWork/PlayerHUD.h"
#include "../GameFrameWork/TestCharacter.h"
#include "../GameFrameWork/TestPlayerController.h"
#include "Components/BoxComponent.h"
#include "Components/Image.h"
#include "NapolitanProject/NapolitanProject.h"
#include "NapolitanProject/BDUI/RequestLetter.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/YJ/NoteUI/ClueInfoWidget.h"
#include "NapolitanProject/YJ/NoteUI/NoteWidget.h"


// Sets default values
AClueActor::AClueActor()
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
void AClueActor::BeginPlay()
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



void AClueActor::IInteract()
{
	//IInteractInterface::Interact();
	LookAt();
}

void AClueActor::LookAt()
{
	
	MainCharacter->SetPlayerState(EPlayerState::UI);
	
	if (!GI->ClueMap.Contains(Clue_ID))
	{
		UE_LOG(LogTemp,Warning,TEXT("ClueData null"))
		return;
	}
	FClueData ClueData= GI->ClueMap[Clue_ID];
	
	// tarray는 0부터 시작
	PlayerHUD->NoteUI->WBP_ClueInfo->ClueSlots[Clue_ID-1]->SetWidgetSwitcher(1);
	
	FString ClueContent =ClueData.Content;
	// ClueData->Content 를 전달

	//방어코드
	
	PlayerHUD->InteractUI->SetRichText_Clue(*ClueContent);
	
	UTexture2D* ClueImage = ClueData.ClueImage;
	if (ClueImage)
	{
		PlayerHUD->InteractUI->SetImgClueContent(ClueImage);
	}
	else
	{
		PlayerHUD->InteractUI->Img_ClueContent->SetVisibility(ESlateVisibility::Hidden);
	}
	//
	
	PlayerHUD->InteractUI->SetVisibleCanvasPanel_Clue(true);
	
	
	// 쪽지 ui 가 나오도록 하기
	 // 자기아이디로 데이터 테이블 읽어와서


	//데이터 테이블에 had으로 표시
	UE_LOG(LogTemp, Warning, TEXT("ClueActorCount: %d%s"),ClueActorCount,*CALLINFO);
	
	if (ClueData.Had){return;}
	else
	{
		ClueData.Had=true;
	
		ClueActorCount++;

		if (ClueActorCount==5)
		{
			GI->UnlockAchievement(TEXT("ClueCollect_ACH"));
		}
	}
	
	//여기서 10번째 면 스팀도전과제 나오도록 하기 
	
}
