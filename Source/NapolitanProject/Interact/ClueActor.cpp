// Fill out your copyright notice in the Description page of Project Settings.


#include "ClueActor.h"

#include "InteractWidget.h"
#include "../GameFrameWork/PlayerHUD.h"
#include "../GameFrameWork/TestCharacter.h"
#include "../GameFrameWork/TestPlayerController.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
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
	MainCharacter =TestPC->GetPawn<ATestCharacter>();
	PlayerHUD =TestPC->GetHUD<APlayerHUD>();
	GI = GetGameInstance<UMyTestGameInstance>();
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
	
	FName Clue_FName = FName(*FString::FromInt(Clue_ID));
	FClueData* ClueData= GI->DT_Clue->FindRow<FClueData>(Clue_FName , TEXT(""));
	if (!ClueData){ UE_LOG(LogTemp,Warning,TEXT("ClueData null")) return;}


	//데이터 테이블에 had으로 표시
	ClueData->Had=true;


	PlayerHUD->NoteUI->WBP_ClueInfo->ClueSlots[Clue_ID]->SetWidgetSwitcher(1);
	
	FString ClueContent =ClueData->Content;
		// ClueData->Content 를 전달
	MainCharacter->SetPlayerState(EPlayerState::UI);
	PlayerHUD->InteractUI->SetRichText_Clue(*ClueContent);

	//
	
	PlayerHUD->InteractUI->SetVisibleCanvasPanel_Clue(true);
	/*FTimerHandle UITimer;
	GetWorldTimerManager().SetTimer(UITimer,[this, CameraLoc, CameraRot]()
	{
		SetActorLocationAndRotation(CameraLoc,CameraRot);
	},0.5f,false);

	FTimerHandle UITimer2;
	GetWorldTimerManager().SetTimer(UITimer2,[this, OriginLoc, OriginRot]()
	{
		SetActorLocationAndRotation(OriginLoc,OriginRot);
		
	},2.0f,false);*/
	
	// 쪽지 ui 가 나오도록 하기
	 // 자기아이디로 데이터 테이블 읽어와서
	
	
}