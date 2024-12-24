// Fill out your copyright notice in the Description page of Project Settings.


#include "ClueActor.h"

#include "InteractWidget.h"
#include "../GameFrameWork/PlayerHUD.h"
#include "../GameFrameWork/TestCharacter.h"
#include "../GameFrameWork/TestPlayerController.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/Image.h"
#include "Components/TextRenderComponent.h"
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

// Called every frame
void AClueActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AClueActor::LookAt()
{
	// 캐릭터의 카메라 시점으로 이동 시켰다가 제자리로
	FVector OriginLoc = GetActorLocation();
	FRotator OriginRot=GetActorRotation();
	FVector CameraLoc =MainCharacter->myArrowComp->GetComponentLocation();
	FRotator CameraRot =MainCharacter->myArrowComp->GetComponentRotation();
	
	MainCharacter->SetPlayerState(EPlayerState::UI);
	FName Clue_FName = FName(*FString::FromInt(Clue_ID));
	FClueData* ClueData= GI->DT_Clue->FindRow<FClueData>(Clue_FName , TEXT(""));
	if (!ClueData)
	{
		// ClueData에 없으면 의뢰서. 의뢰서 ui 나오도록 하기
		if (RequestLetterFactory)
		{
			RequestLetterUI =CreateWidget<URequestLetter>(GetWorld(),RequestLetterFactory);
			if (RequestLetterUI)
			{
				RequestLetterUI->AddToViewport(2);
				MainCharacter->SetPlayerState(EPlayerState::UI);
			}
			
		}
		
		UE_LOG(LogTemp,Warning,TEXT("ClueData null"))
		return;
	}
	

	//데이터 테이블에 had으로 표시
	ClueData->Had=true;

	// tarray는 0부터 시작
	PlayerHUD->NoteUI->WBP_ClueInfo->ClueSlots[Clue_ID-1]->SetWidgetSwitcher(1);
	
	FString ClueContent =ClueData->Content;
	// ClueData->Content 를 전달

	//방어코드
	
	PlayerHUD->InteractUI->SetRichText_Clue(*ClueContent);
	
	

	UTexture2D* ClueImage = ClueData->ClueImage;
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
	
	
}