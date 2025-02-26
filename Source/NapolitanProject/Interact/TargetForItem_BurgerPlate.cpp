// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetForItem_BurgerPlate.h"

#include "EngineUtils.h"
#include "ItemActor.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SlateWrapperTypes.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/NapolitanProject.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/NPC/NPC_Butterfly.h"
#include "NapolitanProject/YJ/Monologue/MonolugueWidget.h"
#include "NapolitanProject/YJ/NoteUI/InventoryWidget.h"

ATargetForItem_BurgerPlate::ATargetForItem_BurgerPlate()
{
	SceneComp1 =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp1"));
	SceneComp1->SetupAttachment(StaticMeshComp);

	SceneComp2 =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp2"));
	SceneComp2->SetupAttachment(StaticMeshComp);

	SceneComp3 =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp3"));
	SceneComp3->SetupAttachment(StaticMeshComp);
	
	SceneComp4 =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp4"));
	SceneComp4->SetupAttachment(StaticMeshComp);

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
	NiagaraComp->SetupAttachment(StaticMeshComp);
}

void ATargetForItem_BurgerPlate::CheckItem(class AItemActor* curItem)
{
	//Super::CheckItem(curItem);

	// curItem 과 비교해야할 id가 많음.
	// 장미:1 거미:3 빵:4 ,5
	if (TargetItemIDArray.Contains(curItem->ItemID))
	{
		CheckItemSuccess();
		int32 ItemID=curItem->ItemID;
		PutDown(ItemID,curItem);
	}
	else
	{
		// 아이템은 있지만 나와 작용하는 아이템이 아님 .
		CheckItemFail();
	}
	
	
}

void ATargetForItem_BurgerPlate::CheckItemSuccess()
{
	
}

void ATargetForItem_BurgerPlate::CheckItemFail()
{
	UE_LOG(LogTemp, Error, TEXT("%s NoItem"),*CALLINFO);
	FString NoItem =FString(TEXT("<Monologue>버거의 재료가 아닌거같다"));
	PlayerHUD->MonolugueWidgetUI->SetVisibility(ESlateVisibility::Visible);
	PlayerHUD->MonolugueWidgetUI->SetText_Dialogue(NoItem);
}

void ATargetForItem_BurgerPlate::NoItem()
{
	UE_LOG(LogTemp, Error, TEXT("%s NoItem"),*CALLINFO);
	FString NoItem =FString(TEXT("<Monologue>버거를 놓을 접시이다"));
	PlayerHUD->MonolugueWidgetUI->SetVisibility(ESlateVisibility::Visible);
	PlayerHUD->MonolugueWidgetUI->SetText_Dialogue(NoItem);
}

void ATargetForItem_BurgerPlate::PutDown(int32 itemID,AItemActor* curItem)
{
	FString SpiderNum;
	int IntSpiderNum;
	switch (itemID)
	{
	case 1:
		// 장미
		curItem->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		curItem->BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3 , ECR_Ignore);
		MainCharacter->curItem = nullptr;
		curItem->AttachToComponent(SceneComp2 , FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		PutDownitemID.Add(itemID);

	// 인벤에서 안보이도록 작업 . 
		PlayerHUD->InventoryUI->InvenSlots[1]->SetIsEnabledBtn();

	//
		curItem->Remove();
		break;
	case 3:

		// 거미는 갯수도 비교해야함 .

		SpiderNum = PlayerHUD->InventoryUI->InvenSlots[3]->Text_Num->GetText().ToString();
		IntSpiderNum = FCString::Atoi(*SpiderNum);
		if (IntSpiderNum >= 10)
		{
			curItem->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
			curItem->BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3 , ECR_Ignore);
			MainCharacter->curItem = nullptr;
			curItem->AttachToComponent(SceneComp3 , FAttachmentTransformRules::SnapToTargetNotIncludingScale);

			PlayerHUD->InventoryUI->InvenSlots[3]->SetIsEnabledBtn();
			PlayerHUD->InventoryUI->InvenSlots[3]->Set_TextNum("");
			PutDownitemID.Add(itemID);
			curItem->Remove();
		}
		else
		{
			UE_LOG(LogTemp , Error , TEXT("%s SpiderNum") , *CALLINFO);
			FString NoItem = FString(TEXT("<Monologue>거미가 부족하다"));
			PlayerHUD->MonolugueWidgetUI->SetVisibility(ESlateVisibility::Visible);
			PlayerHUD->MonolugueWidgetUI->SetText_Dialogue(NoItem);
		}

		break;
	case 4:
		curItem->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		curItem->BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3 , ECR_Ignore);
		MainCharacter->curItem = nullptr;

		if (SceneComp1->GetAttachChildren().IsEmpty())
		{
			curItem->AttachToComponent(SceneComp1 , FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
		else
		{
			curItem->AttachToComponent(SceneComp4 , FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}

		PlayerHUD->InventoryUI->InvenSlots[4]->SetIsEnabledBtn();
		PutDownitemID.Add(itemID);
		curItem->Remove();
		break;
	case 5:
		curItem->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		curItem->BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3 , ECR_Ignore);
		MainCharacter->curItem = nullptr;
		if (SceneComp1->GetAttachChildren().IsEmpty())
		{
			curItem->AttachToComponent(SceneComp1 , FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
		else
		{
			curItem->AttachToComponent(SceneComp4 , FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
		PlayerHUD->InventoryUI->InvenSlots[5]->SetIsEnabledBtn();
		PutDownitemID.Add(itemID);
		curItem->Remove();
		break;
	}

	// 미션체크 확인하기 
	if (PutDownitemID.Num()==4)
	{
		MissionCheck();
	}
}

void ATargetForItem_BurgerPlate::MissionCheck()
{
	UE_LOG(LogTemp,Warning,TEXT("%s 거미버거 퀘스트 달성"),*CALLINFO);
	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,ECR_Ignore);

	// 소리나오도록 하기
	if (MissionClearSoundWave)
	{
		UGameplayStatics::PlaySound2D(this, MissionClearSoundWave);
	}

	NiagaraComp->Activate();

	
	
	FTimerHandle Timer1;
	//바로 나비의 대사 나오도록 하기
	GetWorld()->GetTimerManager().SetTimer(Timer1,[this]()
	{

		BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,ECR_Ignore);
		
		// 만든 버거는 사라지도록 하기
		USceneComponent* item4  =SceneComp4->GetChildComponent(0);
		item4->SetHiddenInGame(true);
		USceneComponent* item1  =SceneComp1->GetChildComponent(0);
		item1->SetHiddenInGame(true);
		USceneComponent* item2  =SceneComp2->GetChildComponent(0);
		item2->SetHiddenInGame(true);
		USceneComponent* item3  =SceneComp3->GetChildComponent(0);
		item3->SetHiddenInGame(true);

		
		// 2층문이 열리도록 하기
		for (TActorIterator<ANPC_Butterfly> It(GetWorld(), ANPC_Butterfly::StaticClass()); It; ++It)
		{
			NPC_Butterfly=*It;
		}
		if (NPC_Butterfly)
		{
			NPC_Butterfly->State=2;
			NPC_Butterfly->TargetForItem_BurgerPlate=this;
		}
	},3.0f,false);

	FTimerHandle Timer4;
	

	FTimerHandle Timer2;
	GetWorld()->GetTimerManager().SetTimer(Timer2 , [this]()
	{
		
		TestPC->curNPC =NPC_Butterfly;
		TestPC->StartEndNPCDialougue(true);
		TestPC->SetNPCDialougueText(0);
		// 나의 상태 변화
		MainCharacter->SetPlayerState(EPlayerState::Talking);
	} , 5.0f , false);

	
}
