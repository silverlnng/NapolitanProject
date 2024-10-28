// Fill out your copyright notice in the Description page of Project Settings.


#include "EventComponent.h"

#include "TestPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"
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
	// ...
	
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
	if (str=="RedDosentStart")
	{
		Event_RedDosent(str,content);
	}
	else if ("RedDosentEnd")
	{
		Event_RedDosent(str,content);
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

