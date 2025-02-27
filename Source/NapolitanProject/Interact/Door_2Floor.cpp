// Fill out your copyright notice in the Description page of Project Settings.


#include "Door_2Floor.h"

#include "Components/BoxComponent.h"
#include "Components/SlateWrapperTypes.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/YJ/Monologue/MonolugueWidget.h"

void ADoor_2Floor::BeginPlay()
{
	Super::BeginPlay();
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ADoor_2Floor::BeginOverlap);
	
	GI=GetGameInstance<UMyTestGameInstance>();
	// 청소부 만났으면 바인드해제하기
	// QuestSlots
	FString QuestText =FString(TEXT("머리를 찾아주기"));
	if (GI&&GI->QuestSlots.Contains(QuestText))
	{
		BoxComp->OnComponentBeginOverlap.Clear();
		JustRotateDoor();
	}
	
}


void ADoor_2Floor::UnBindBeginOverlap()
{
	//청소부 만나고 나면 해제하기
	BoxComp->OnComponentBeginOverlap.Clear();
	// 문열리도록 하기 
	StartRotateDoor();
}

void ADoor_2Floor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Super::OnBeginOverlap(OverlappedComponent , OtherActor , OtherComp , OtherBodyIndex , bFromSweep , SweepResult);
	// 독백대사 나오도록 하기
	if(OtherActor)
	{
		
		ATestCharacter* MainCharacter = Cast<ATestCharacter>(OtherActor);
		if (MainCharacter)
		//퀘스트 있는지 검사
		{
			FString NoEnter =FString(TEXT("<Monologue>2층 전시관은 닫혀있어서 갈수없다"));
			MainCharacter->PlayerHUD->MonolugueWidgetUI->SetVisibility(ESlateVisibility::Visible);
			MainCharacter->PlayerHUD->MonolugueWidgetUI->SetText_Dialogue(NoEnter);
		}
	}
}

void ADoor_2Floor::RotateDoor()
{
	Super::RotateDoor();
}

void ADoor_2Floor::RotateDoor2()
{
	Super::RotateDoor2();
}

void ADoor_2Floor::StartRotateDoor()
{
	Super::StartRotateDoor();
}

void ADoor_2Floor::JustRotateDoor()
{
	Super::RotateDoor2();
	Super::RotateDoor();
}
