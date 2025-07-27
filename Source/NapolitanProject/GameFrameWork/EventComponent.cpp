// Fill out your copyright notice in the Description page of Project Settings.


#include "EventComponent.h"


#include "MyTestGameInstance.h"
#include "PlayerHUD.h"

#include "TestCharacter.h"
#include "TestPlayerController.h"
#include "Components/RichTextBlock.h"
#include "NapolitanProject/NPC/Butterfly/Command/ButterflyCloseUpCommand.h"
#include "NapolitanProject/NPC/Butterfly/Command/ButterflyQuestStartCommand.h"
#include "NapolitanProject/NPC/Butterfly/Command/ButterflyQuestCompletedCommand.h"
#include "NapolitanProject/NPC/Butterfly/Command/ButterflyQuestRewardCommand.h"
#include "NapolitanProject/NPC/Cleaner/Command/CleanerQuestCompletedCommand.h"
#include "NapolitanProject/NPC/Cleaner/Command/CleanerQuestStartCommand.h"
#include "NapolitanProject/NPC/Curator/Command/CuratorCompletedCommand.h"
#include "NapolitanProject/NPC/Curator/Command/CuratorLightEffectCommand.h"
#include "NapolitanProject/NPC/Curator/Command/CuratorUICommand.h"
#include "NapolitanProject/NPC/Docent/Command/DocentCloseUpCommand.h"
#include "NapolitanProject/NPC/Docent/Command/DocentDetectStartCommand.h"
#include "NapolitanProject/NPC/Docent/Command/DocentEndCommand.h"
#include "NapolitanProject/NPC/Docent/Command/DocentNoteUICommand.h"
#include "NapolitanProject/NPC/LeeSeo/Command/LeeSeoFirstUICommand.h"
#include "NapolitanProject/NPC/LeeSeo/Command/LeeSeoSecondUICommand.h"
#include "NapolitanProject/NPC/Security/SecurityCompletedCommand.h"
#include "NapolitanProject/YJ/EventWidget.h"


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
	
	if (TestPC&&TestPC->GI)
	{
		GI =TestPC->GI;
	}
	// 6 : AChaseStatue (큐레이터)
	// 8 : 나비
	// 1 : 김영수
	// 2 : 도슨트
	// 3 : 노인 (이동준)
	// 4 : 검은경비원
	// 5 : 흰 청소부
	// 7 : 이서

	//CommandMap 에 커맨드를 연결시키기
	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle,this,&UEventComponent::CommandAdd,0.5f, false);
	
}

void UEventComponent::CommandAdd()
{
	// 게임모드의 NPCArray 가져오기
	// 몇몇 npc는 다른 게임모드를 넣어줘야함 !

	// 이서 Command 연결
	CommandMap.Add(
		"LeeSeoFirstUI" ,
		MakeShared<LeeSeoFirstUICommand>(TestPC , MainCharacter , PlayerHUD , GetWorld()));
	CommandMap.Add(
		"LeeSeoSecondUI" ,
		MakeShared<LeeSeoSecondUICommand>(TestPC , MainCharacter , PlayerHUD , GetWorld()));

	// 큐레이터 command 연결
	CommandMap.Add(
		"CuratorLightEffect" ,
		MakeShared<CuratorLightEffectCommand>(TestPC , MainCharacter , PlayerHUD , GetWorld()));

	CommandMap.Add(
		"CuratorCompleted" ,
		MakeShared<CuratorCompletedCommand>(TestPC , MainCharacter , PlayerHUD , GetWorld(),GI));
	
	CommandMap.Add(
			"CuratorUI" ,
			MakeShared<CuratorUICommand>(TestPC , MainCharacter , PlayerHUD , GetWorld()));
	// 도슨트

	CommandMap.Add(
		"DocentDetectStart" ,
		MakeShared<DocentDetectStartCommand>(TestPC , MainCharacter,GetWorld()));
	CommandMap.Add(
		"DocentNoteUI" ,
		MakeShared<DocentNoteUICommand>(TestPC , MainCharacter , PlayerHUD , GetWorld()));
	CommandMap.Add(
		"DocentCloseUp" ,
		MakeShared<DocentCloseUpCommand>(TestPC , MainCharacter , GetWorld()));
	CommandMap.Add(
		"DocentPlayEnd" ,
		MakeShared<DocentEndCommand>(TestPC , MainCharacter , GetWorld()));

	
	CommandMap.Add(
		"SecurityCompleted" ,
		MakeShared<SecurityCompletedCommand>(TestPC , MainCharacter , PlayerHUD , GetWorld(),GI));


	CommandMap.Add(
		"CleanerQuestStart" ,
		MakeShared<CleanerQuestStartCommand>(TestPC , MainCharacter , PlayerHUD , GetWorld(),GI));
	CommandMap.Add(
		"CleanerQuestCompleted" ,
		MakeShared<CleanerQuestCompletedCommand>(TestPC , MainCharacter , PlayerHUD , GetWorld(),GI));
	
	CommandMap.Add(
		"ButterflyQuestStart" ,
		MakeShared<ButterflyQuestStartCommand>(TestPC , MainCharacter , PlayerHUD , GetWorld(),GI));
	CommandMap.Add(
		"ButterflyQuestCompleted" ,
		MakeShared<ButterflyQuestCompletedCommand>(TestPC , MainCharacter , PlayerHUD , GetWorld()));
	CommandMap.Add(
		"ButterflyQuestReward" ,
		MakeShared<ButterflyQuestRewardCommand>(TestPC , MainCharacter , PlayerHUD , GetWorld()));
	CommandMap.Add(
	"ButterflyCloseUp" ,
	MakeShared<ButterflyCloseUpCommand>(TestPC , MainCharacter, GetWorld()));

	GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Green , FString::Printf(TEXT("ButterflyCommandMap")));
	
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

void UEventComponent::StartEvent_(FString& EventKey)
{
	if (auto FoundCommand = CommandMap.Find(EventKey))
	{
		(*FoundCommand)->Execute();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,*EventKey);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,FString::Printf(TEXT("CommandMap EventKey 없음")));
	}
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

