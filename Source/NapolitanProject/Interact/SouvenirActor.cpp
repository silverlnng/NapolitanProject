// Fill out your copyright notice in the Description page of Project Settings.


#include "SouvenirActor.h"

#include "EngineUtils.h"
#include "ExitDoor_LeeSeo.h"
#include "InteractWidget.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/SaveGISubsystem.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/YJ/NoteUI/EscapeRuleWidget.h"
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
	if (!TestPC) {return;}
	MainCharacter =TestPC->GetPawn<ATestCharacter>();
	PlayerHUD=TestPC->GetHUD<APlayerHUD>();
	GI = GetGameInstance<UMyTestGameInstance>();
	SaveGI=GI->GetSubsystem<USaveGISubsystem>();
	
	if(M_Overlay)
	{
		StaticMeshComp->SetOverlayMaterial(M_Overlay);
	}
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

void ASouvenirActor::IInteract()
{
	OnPickup();
}

void ASouvenirActor::OnPickup()
{
	StaticMeshComp->SetHiddenInGame(true);
	SaveGI->AcquireSouvenir.Add(GetSouvenirID());
	// 얻을때마다 갯수셀꺼여서 초기화 
	SaveGI->AcquireSouvenirNum=0;
	
	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,ECR_Ignore);
	FName SouvenirName_ = FName(*FString::FromInt(GetSouvenirID()));
			
	FSouvenirData* SouvenirData= GI->DT_SouvenirData->FindRow<FSouvenirData>(SouvenirName_ , TEXT(""));
	if (SouvenirData)
	{
		SouvenirData->Had=true;
	}

	if (GetSouvenirID()%2==0) // 짝수라면 
	{
		PlayerHUD->NoteUI->WBP_Souvenir->curPage=GetSouvenirID()-1;
	
		TestPC->SetSouvenirUICurNumber(GetSouvenirID()-1);
	}
	else
	{
		PlayerHUD->NoteUI->WBP_Souvenir->curPage=GetSouvenirID();
	
		TestPC->SetSouvenirUICurNumber(GetSouvenirID());
	}

	
	

	FTimerHandle SouvenirTimer;
	GetWorldTimerManager().SetTimer(SouvenirTimer, [this]()
	{
		PlayerHUD->InteractUI->GetSouvenirEvent(GetSouvenirName());
	}, 0.2f, false);


	// 지금까지 얻은 유물의 갯수가 3개일때 , 5개일 때를 각각 체크하기

	// SouvenirData 에서 had 가 true인것만 검색

	for (int i = 0; i < GI->SouvenirDataRowNames.Num(); i++)
	{
		FSouvenirData* SouvenirData_ = GI->DT_SouvenirData->FindRow<FSouvenirData>(GI->SouvenirDataRowNames[i] , TEXT(""));
		if (SouvenirData_->Had==true)
		{
			 SaveGI->AcquireSouvenirNum++;
			FString num=FString::FromInt(SaveGI->AcquireSouvenirNum);
			FString temp="AcquireSouvenirNum"+num;
			 GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *temp);
		}
	}

	// GI->AcquireSouvenirNum 을 받아서 
	PlayerHUD->NoteUI->WBP_EscapeRule->SetAcquireImage(SaveGI->AcquireSouvenirNum,SouvenirData->thumnail);
	PlayerHUD->NoteUI->WBP_EscapeRule->SetKeyImage(SaveGI->AcquireSouvenirNum);

	PlayerHUD->NoteUI->SelectContent(5);

/////////////////// 유물 3개 모으는 순간 
	/*if (GI->AcquireSouvenirNum==3)
	{
		// npc 이서 방문열리는 쪽으로 화면 전환 하는 이벤트 발생시키기
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "유물3개 획득");
		
		
		// ui 이벤트를 발생시키기
		FTimerHandle UITimer5;

		GetWorld()->GetTimerManager().SetTimer(UITimer5,[this]()
		{
			FString QuestText =FString(TEXT("숨겨진 공간을 찾아보자"));
			PlayerHUD->InteractUI->AddQuestSlot(4,QuestText);
		},1.5f,false);

		AExitDoor_LeeSeo* Door=nullptr;
		for (TActorIterator<AExitDoor_LeeSeo> It(GetWorld(), AExitDoor_LeeSeo::StaticClass()); It; ++It)
		{
			Door = *It;
		}
		if (Door)
		{
			Door->BindBeginOverlap();
		}
	}*/

	
	FTimerHandle SouvenirTimer2;
	GetWorldTimerManager().SetTimer(SouvenirTimer2, [this]()
	{
		this->Destroy();
	}, 5.0, false);
	
}


