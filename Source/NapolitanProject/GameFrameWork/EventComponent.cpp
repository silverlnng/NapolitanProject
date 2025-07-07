// Fill out your copyright notice in the Description page of Project Settings.


#include "EventComponent.h"

#include "EngineUtils.h"
#include "MyTestGameInstance.h"
#include "PlayerHUD.h"
#include "SaveGISubsystem.h"
#include "TestCharacter.h"
#include "TestGameModeBase.h"
#include "TestPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/Interact/Door_2Floor.h"
#include "NapolitanProject/Interact/InteractWidget.h"
#include "NapolitanProject/Interface/Command/ButterflyQuestStartCommand.h"
#include "NapolitanProject/NPC/ChaseStatue.h"
#include "NapolitanProject/NPC/NPCCharacter.h"
#include "NapolitanProject/NPC/Butterfly/NPC_Butterfly.h"
#include "NapolitanProject/YJ/EventWidget.h"
#include "NapolitanProject/YJ/DialogueUI/NPCResultWidget.h"
#include "NapolitanProject/YJ/NoteUI/NoteWidget.h"
#include "NapolitanProject/YJ/NoteUI/NPCInfoWidget.h"

// Sets default values for this component's properties
UEventComponent::UEventComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEventComponent::BeginPlay()
{
	Super::BeginPlay();
	TestPC=GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	MainCharacter=Cast<ATestCharacter>(TestPC->GetPawn());
	PlayerHUD=TestPC->GetHUD<APlayerHUD>();
	
	// 게임모드의 NPCArray 가져오기

	ATestGameModeBase* TestGM = Cast<ATestGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!TestGM) return;

	if (TestGM->NPCArray.Contains(8))
	{
		CommandMap.Add(
		   "ButterflyQuestStart",
		   MakeShared<ButterflyQuestStartCommand>(TestGM->NPCArray[8], TestPC,MainCharacter,PlayerHUD,GetWorld())
	   );

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("CommandMap")));
	}
	
}

void UEventComponent::InitializeComponent()
{
	Super::InitializeComponent();
	//TestPC=GetOwner<ATestPlayerController>();
	TestPC=GetWorld()->GetFirstPlayerController<ATestPlayerController>();
}


// Called every frame
void UEventComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEventComponent::StartEvent(FString& str,const FString& content)
{
	const FName NameKey(str);
	// NPCEventManage에서 한번 조회 
	if (SaveGI&&!(SaveGI->NPCEventManage.IsEmpty())&&SaveGI->NPCEventManage.Contains(NameKey))
	{
		return;
	}
		
	if (str=="RedDosentStart")
	{
		Event_RedDosent(str,content);
	}
	else if (str=="RedDosentEnd")
	{
		Event_RedDosent(str,content);
	}
	else if (str=="NPCFinalEvent")
	{
		NPCFinalEvent();
		// 선택지 누르고 결과가 아니라 그냥 대사 마지막에 npc의  ResultEvent 호출시키고 싶을때 
	}
	else if (str=="CleanerQuest")
	{
		// 청소부의 퀘스트 함수
		Event_Cleaner_Start();
		SaveGI->NPCEventManage.Add(NameKey);
	}
	else if (str=="CleanerQuestCompleted")
	{
		// 청소부의 퀘스트 완료 함수
		Event_Cleaner_Completed();
		SaveGI->NPCEventManage.Add(NameKey);
	}
	else if (str=="OldmanClue")
	{
		
		FTimerHandle UITimer;
		GetWorld()->GetTimerManager().SetTimer(UITimer,[this]()
		{
			// 청소부의 퀘스트 완료 함수
			Event_Oldman();
		},3.0f,false);
		SaveGI->NPCEventManage.Add(NameKey);
	}
	else if (str=="ButterflyQuest")
	{
		Event_Butterfly_Start();
		SaveGI->NPCEventManage.Add(NameKey);
	}
	else if (str=="ButterflyQuestCompleted")
	{
		Event_Butterfly_Completed();
		SaveGI->NPCEventManage.Add(NameKey);
	}
	else if (str=="ButterflyQuestReward")
	{
		Event_Butterfly_QuestReward();
		SaveGI->NPCEventManage.Add(NameKey);
	}
	else if (str=="LeeSeoFirstUI")
	{
		Event_LeeSeo_FirstUI();
		SaveGI->NPCEventManage.Add(NameKey);
	}
	else if (str=="LeeSeoSecondUI")
	{
		Event_LeeSeo_SecondUI();
		SaveGI->NPCEventManage.Add(NameKey);
	}
	else if (str=="CuratorLightEffect")
	{
		//
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Event_Curator_LightEffect")));
		Event_Curator_LightEffect();
		// 조명 깜빡임 효과를 주기 
	}
	else if (str=="CuratorCompleted")
	{
		Event_Curator_Completed();
		SaveGI->NPCEventManage.Add(NameKey);
	}
}

void UEventComponent::NPCFinalEvent()
{
	TestPC->CallCurNPCResultEvent(9);
	//MainCharacter->curNPC->ResultEvent(9);
}

void UEventComponent::Event_RedDosent(FString& str,const FString& content)
{
	// 위젯하나 만들고
	if (str=="RedDosentStart")
	{
		if (TestPC&&TestPC->EventWidgetFactory)
		{
			TestPC->EventUI=CreateWidget<UEventWidget>(GetWorld(),TestPC->EventWidgetFactory);
		}
		if (TestPC->EventUI)
		{
			TestPC->EventUI->AddToViewport();
			CurrentText="";
			FullText=*content;
			// 앞쪽 태그 찾아내기 >
			int32 StartTagEnd = FullText.Find(TEXT(">"))+1;
			// 앞쪽 태그 잘라낸 문장
			RemoveTags=FullText.Mid(StartTagEnd,FullText.Len());
			// 태그
			StartTag = "<Red_Big>";
			
			GetWorld()->GetTimerManager().SetTimer(TextUpdateTimerHandle,this,&UEventComponent::UpdateText,0.02f, true);
			
		}
	}
	else if (str=="RedDosentEnd")
	{
		// 만든 위젯 삭제
		if (TestPC->EventUI && TestPC->EventUI->IsInViewport())
		{
			TestPC->EventUI->RemoveFromParent();
			TestPC->EventUI = nullptr;  // 포인터 초기화로 메모리 누수 방지
		}
	}
}

void UEventComponent::Event_Docent_NoteUI()
{
	TestPC->StartEndNPCDialougue(false);
	TestPC->EndResult();
	// 머리잡았을때 경비원의 노트ui 나오고 단서 추가하도록 하기
	MainCharacter->SetPlayerState(EPlayerState::UI);

	// 시간지연
	FTimerHandle UITimer;

	GetWorld()->GetTimerManager().SetTimer(UITimer,[this]()
	{
		PlayerHUD->NoteUI->SetVisibility(ESlateVisibility::Visible);

		PlayerHUD->NoteUI->OnClickBtn_Btn_Docent();
		
	},1.0f,false);

	FTimerHandle UITimer2;

	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this]()
	{
		PlayerHUD->NoteUI->WBP_NPCInfo->SetForcus_ScrollBox_Docent(2,1);
	},1.5f,false);
}

void UEventComponent::Event_Cleaner_Start()
{

	// 대화 창 닫고
	TestPC->StartEndNPCDialougue(false);

	MainCharacter->SetPlayerState(EPlayerState::UI);
	
	// 시간지연
	FTimerHandle UITimer;

	GetWorld()->GetTimerManager().SetTimer(UITimer,[this]()
	{
		PlayerHUD->NoteUI->SetVisibility(ESlateVisibility::Visible);

		PlayerHUD->NoteUI->OnClickBtn_Btn_Cleaner();
		
	},2.0f,false);
	

	// 시간지연
	FTimerHandle UITimer2;

	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this]()
	{
		PlayerHUD->NoteUI->WBP_NPCInfo->SetForcus_ScrollBox_Cleaner(1,1);
	},2.5f,false);

	

	FTimerHandle UITimer3;

	GetWorld()->GetTimerManager().SetTimer(UITimer3,[this]()
	{
		FString QuestText =FString(TEXT("머리를 찾아주기"));
		PlayerHUD->InteractUI->AddQuestSlot(2,QuestText);
	},8.0f,false);

	FTimerHandle Timer4;

	GetWorld()->GetTimerManager().SetTimer(Timer4,[this]()
	{
		// 2층문이 열리도록 하기
		for (TActorIterator<ADoor_2Floor> It(GetWorld(), ADoor_2Floor::StaticClass()); It; ++It)
		{
			Door_2Floor=*It;
		}
		if(Door_2Floor)
		{
			Door_2Floor->UnBindBeginOverlap();
		}
	},14.0f,false);
	
	

}

void UEventComponent::Event_Cleaner_Completed()
{
	// 대화 창 닫고
	TestPC->StartEndNPCDialougue(false);
	TestPC->EndResult();
	MainCharacter->SetPlayerState(EPlayerState::UI);
	
	// 시간지연
	FTimerHandle UITimer;

	GetWorld()->GetTimerManager().SetTimer(UITimer,[this]()
	{
		PlayerHUD->NoteUI->SetVisibility(ESlateVisibility::Visible);

		PlayerHUD->NoteUI->OnClickBtn_Btn_Cleaner();
		
	},2.0f,false);
	

	// 시간지연
	FTimerHandle UITimer2;

	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this]()
	{
		PlayerHUD->NoteUI->WBP_NPCInfo->SetForcus_ScrollBox_Cleaner(2,2);
	},2.5f,false);

	FTimerHandle UITimer3;

	GetWorld()->GetTimerManager().SetTimer(UITimer3,[this]()
	{
		MainCharacter->SetPlayerState(EPlayerState::UI);
		
	},6.0f,false);

	FTimerHandle UITimer4;
	
	GetWorld()->GetTimerManager().SetTimer(UITimer4,[this]()
	{
		FString QuestText =FString(TEXT("머리를 가져다 주자"));
		PlayerHUD->InteractUI->RemoveQuestSlot(QuestText);
	},8.0f,false);


}

void UEventComponent::Event_Security_Completed()
{
	TestPC->StartEndNPCDialougue(false);
	TestPC->EndResult();
	// 머리잡았을때 경비원의 노트ui 나오고 단서 추가하도록 하기
	MainCharacter->SetPlayerState(EPlayerState::UI);

	// 시간지연
	FTimerHandle UITimer;

	GetWorld()->GetTimerManager().SetTimer(UITimer,[this]()
	{
		PlayerHUD->NoteUI->SetVisibility(ESlateVisibility::Visible);

		PlayerHUD->NoteUI->OnClickBtn_Btn_Security();
		
	},2.0f,false);

	FTimerHandle UITimer2;

	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this]()
	{
		PlayerHUD->NoteUI->WBP_NPCInfo->SetForcus_ScrollBox_Security(2,1);
	},2.5f,false);
}

void UEventComponent::Event_Oldman()
{
	TestPC->StartEndNPCDialougue(false);
	TestPC->EndResult();
	
	MainCharacter->SetPlayerState(EPlayerState::UI);

	// 시간지연
	FTimerHandle UITimer;

	GetWorld()->GetTimerManager().SetTimer(UITimer,[this]()
	{
		PlayerHUD->NoteUI->SetVisibility(ESlateVisibility::Visible);

		PlayerHUD->NoteUI->OnClickBtn_Btn_Oldman();
		
	},2.0f,false);

	FTimerHandle UITimer2;

	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this]()
	{
		PlayerHUD->NoteUI->WBP_NPCInfo->SetForcus_ScrollBox_Oldman(2,1);
	},2.5f,false);

	FTimerHandle UITimer3;
	GetWorld()->GetTimerManager().SetTimer(UITimer3,[this]()
	{
		MainCharacter->SetPlayerState(EPlayerState::UI);
	},6.0f,false);
}

void UEventComponent::Event_Butterfly_Start()
{
	PlayerHUD->NPCResultUI->SetVisibility(ESlateVisibility::Hidden);
	PlayerHUD->NPCResultUI->curOrder=0;
	MainCharacter->SetPlayerState(EPlayerState::UI);
	
	// 시간지연
	FTimerHandle UITimer;

	GetWorld()->GetTimerManager().SetTimer(UITimer,[this]()
	{
		PlayerHUD->NoteUI->SetVisibility(ESlateVisibility::Visible);

		PlayerHUD->NoteUI->OnClickBtn_Btn_Butterfly();
	},2.0f,false);

	FTimerHandle UITimer2;

	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this]()
	{
		PlayerHUD->NoteUI->WBP_NPCInfo->SetForcus_ScrollBox_Butterfly(1,1);
	},2.5f,false);
	

	FTimerHandle UITimer3;

	GetWorld()->GetTimerManager().SetTimer(UITimer3,[this]()
	{
		FString QuestText =FString(TEXT("거미버거를 만들어주기"));
		PlayerHUD->InteractUI->AddQuestSlot(4,QuestText);
	},8.0f,false);
	
	FTimerHandle UITimer4;
	GetWorld()->GetTimerManager().SetTimer(UITimer4,[this]()
	{
		MainCharacter->SetPlayerState(EPlayerState::UI);
	},6.0f,false);

	ANPC_Butterfly* NPC_Butterfly= Cast<ANPC_Butterfly>(TestPC->curNPC);
	if (NPC_Butterfly)
	{
		NPC_Butterfly->State=2; //현재 나비여서 state 증가시키기 
	}
}

void UEventComponent::Event_Butterfly_Completed()
{
	FTimerHandle UITimer;
	
	GetWorld()->GetTimerManager().SetTimer(UITimer,[this]()
	{
		FString QuestText =FString(TEXT("거미버거를 만들어주기"));
		PlayerHUD->InteractUI->RemoveQuestSlot(QuestText);
	},1.0f,false);
	
}

void UEventComponent::Event_Butterfly_QuestReward()
{
	TestPC->StartEndNPCDialougue(false);
	
	// 스폰시키고
	ANPC_Butterfly* NPC_Butterfly= Cast<ANPC_Butterfly>(TestPC->curNPC);
	if (NPC_Butterfly)
	{
		NPC_Butterfly->SpawnItems();
	}

	MainCharacter->SetPlayerState(EPlayerState::UI);

	// 시간지연
	FTimerHandle UITimer;

	GetWorld()->GetTimerManager().SetTimer(UITimer,[this]()
	{
		PlayerHUD->NoteUI->SetVisibility(ESlateVisibility::Visible);

		PlayerHUD->NoteUI->OnClickBtn_Btn_Butterfly();
		
	},2.0f,false);

	FTimerHandle UITimer2;

	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this]()
	{
		PlayerHUD->NoteUI->WBP_NPCInfo->SetForcus_ScrollBox_Butterfly(2,3);
	},2.5f,false);
	
}

void UEventComponent::Event_LeeSeo_FirstUI()
{
	TestPC->StartEndNPCDialougue(false);
	
	MainCharacter->SetPlayerState(EPlayerState::UI);

	// 시간지연
	FTimerHandle UITimer;

	GetWorld()->GetTimerManager().SetTimer(UITimer,[this]()
	{
		PlayerHUD->NoteUI->SetVisibility(ESlateVisibility::Visible);

		PlayerHUD->NoteUI->OnClickBtn_Btn_Girl();
		
	},2.0f,false);

	FTimerHandle UITimer2;

	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this]()
	{
		PlayerHUD->NoteUI->WBP_NPCInfo->SetForcus_ScrollBox_LeeSeo(1,1);
	},2.5f,false);
}

void UEventComponent::Event_LeeSeo_SecondUI()
{
	TestPC->StartEndNPCDialougue(false);
	
	MainCharacter->SetPlayerState(EPlayerState::UI);

	// 시간지연
	FTimerHandle UITimer;

	GetWorld()->GetTimerManager().SetTimer(UITimer,[this]()
	{
		PlayerHUD->NoteUI->SetVisibility(ESlateVisibility::Visible);

		PlayerHUD->NoteUI->OnClickBtn_Btn_Girl();
		
	},2.0f,false);

	FTimerHandle UITimer2;

	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this]()
	{
		PlayerHUD->NoteUI->WBP_NPCInfo->SetForcus_ScrollBox_LeeSeo(2,2);
	},2.5f,false);
}

void UEventComponent::Event_Curator_LightEffect()
{
	// 조명 깜빡임 효과를 주기
	AChaseStatue* Curator=nullptr;
	if (TestPC->curNPC)
	{
		Curator=Cast<AChaseStatue>(TestPC->curNPC);
		if (Curator)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Curator->LightEffect")));
			Curator->LightEffect();
		}
	}
	
}


void UEventComponent::Event_Curator_Completed()
{
	TestPC->StartEndNPCDialougue(false);
	
	MainCharacter->SetPlayerState(EPlayerState::UI);

	// 시간지연
	FTimerHandle UITimer;

	GetWorld()->GetTimerManager().SetTimer(UITimer,[this]()
	{
		PlayerHUD->NoteUI->SetVisibility(ESlateVisibility::Visible);

		PlayerHUD->NoteUI->OnClickBtn_Btn_Curator();
		
	},2.0f,false);

	FTimerHandle UITimer2;

	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this]()
	{
		PlayerHUD->NoteUI->WBP_NPCInfo->SetForcus_ScrollBox_Curator(2,2);
	},2.5f,false);
}

void UEventComponent::UpdateText()
{
	if (CurrentText.Len() >= RemoveTags.Len())
	{
		GetWorld()->GetTimerManager().ClearTimer(TextUpdateTimerHandle);
		return;
	}
	
	// 한 글자씩 추가
	
	CurrentText += RemoveTags.Mid(CurrentText.Len(), 1);

	//CurrentText.Append("</>");
	
	// TextBlock에 적용
	if (TestPC&&TestPC->EventUI&&TestPC->EventUI->RichText_Event)
	{
		TestPC->EventUI->RichText_Event->SetText(FText::FromString(StartTag+CurrentText+TEXT("</>")));
	}
}

