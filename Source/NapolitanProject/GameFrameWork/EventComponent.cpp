// Fill out your copyright notice in the Description page of Project Settings.


#include "EventComponent.h"

#include "MyTestGameInstance.h"
#include "PlayerHUD.h"
#include "TestCharacter.h"
#include "TestPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"
#include "NapolitanProject/Interact/InteractWidget.h"
#include "NapolitanProject/YJ/EventWidget.h"
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
	// ...
	PlayerHUD=TestPC->GetHUD<APlayerHUD>();
	
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
	if (GI&&!(GI->NPCEventManage.IsEmpty())&&GI->NPCEventManage.Contains(NameKey))
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
	else if (str=="CleanerQuest")
	{
		// 청소부의 퀘스트 함수
		Event_Cleaner_Start();
		GI->NPCEventManage.Add(NameKey);
	}
	else if (str=="CleanerQuestCompleted")
	{
		// 청소부의 퀘스트 완료 함수
		Event_Cleaner_Completed();
		GI->NPCEventManage.Add(NameKey);
	}
	else if (str=="OldmanClue")
	{
		
		FTimerHandle UITimer;
		GetWorld()->GetTimerManager().SetTimer(UITimer,[this]()
		{
			// 청소부의 퀘스트 완료 함수
			Event_Oldman();
		},3.0f,false);
		GI->NPCEventManage.Add(NameKey);
	}
	else if (str=="ButterflyQuest")
	{
		Event_Butterfly_Start();
		GI->NPCEventManage.Add(NameKey);
	}
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
		PlayerHUD->InteractUI->RemoveQuestSlot("머리를 가져다 주자");
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
	TestPC->StartEndNPCDialougue(false);
	TestPC->EndResult();

	
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

