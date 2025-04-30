// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

#include "EngineUtils.h"
#include "MyTestGameInstance.h"
#include "../YJ/DeadEndingWidget.h"
#include "../Interact/InteractWidget.h"
#include "../YJ/DialogueUI/NPCDialogueWidget.h"
#include "Blueprint/UserWidget.h"
#include "NYS_Choice.h"
#include "YSEvanceUI.h"
#include "../YJ/NoteUI/NoteWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "NapolitanProject/NapolitanProject.h"
#include "NapolitanProject/YJ/ESCWidget.h"
#include "NapolitanProject/YJ/DialogueUI/NPCResultWidget.h"
#include "NapolitanProject/YJ/Monologue/MonolugueWidget.h"
#include "NapolitanProject/YJ/NoteUI/ClueInfoWidget.h"
#include "NapolitanProject/YJ/NoteUI/ClueSlotWidget.h"
#include "NapolitanProject/YJ/NoteUI/InventoryWidget.h"
#include "NapolitanProject/YJ/NoteUI/NPCInfoWidget.h"
#include "NapolitanProject/YJ/SaveUI/LoadScreenWidget.h"

// hud 의 BeginPlay 는 저장되어있는거 로드된후에 실행됨 
void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	
	GI =GetGameInstance<UMyTestGameInstance>();
	
	UESC_UI =CreateWidget<UESCWidget>(GetWorld(),UESCWidgetFactory);
	if (UESC_UI)
	{
		UESC_UI->AddToViewport(3);
		UESC_UI->SetVisibility(ESlateVisibility::Hidden);
	}
	
	NoteUI =CreateWidget<UNoteWidget>(GetWorld(),NoteWidgetFactory);
	if (NoteUI)
	{
		NoteUI->AddToViewport(2);
		NoteUI->SetVisibility(ESlateVisibility::Hidden);
	}

	// 초반에 slot 을 생성해주기
	InventoryUI =CreateWidget<UInventoryWidget>(GetWorld(),InventoryFactory);
	if (InventoryUI)
	{
		InventoryUI->AddToViewport(1);
		InventoryUI->SetVisibility(ESlateVisibility::Hidden);
		InventoryUI->GI=GI;
	}
	
	NPCDialogueUI =CreateWidget<UNPCDialogueWidget>(GetWorld(),NPCDialogueWidgetFactory);
	if (NPCDialogueUI)
	{
		NPCDialogueUI->AddToViewport(1);
		NPCDialogueUI->SetVisibility(ESlateVisibility::Hidden);
	}

	NPCResultUI =CreateWidget<UNPCResultWidget>(GetWorld(),NPCResultWidgetFactory);
	if (NPCResultUI)
	{
		NPCResultUI->AddToViewport(1);
		NPCResultUI->SetVisibility(ESlateVisibility::Hidden);
	}

	
	InteractUI=CreateWidget<UInteractWidget>(GetWorld(),InteractWidgetFactory);
	if (InteractUI)
	{
		InteractUI->AddToViewport();
		InteractUI->SetVisibleCrossHair(true);
		InteractUI->SetVisibleHBox(false);
		InteractUI->GI=GI;
	}

	DeadEndingWidgetUI=CreateWidget<UDeadEndingWidget>(GetWorld(),DeadEndingWidgetFactory);
	if (DeadEndingWidgetUI)
	{
		DeadEndingWidgetUI->AddToViewport(3);
		DeadEndingWidgetUI->SetVisibility(ESlateVisibility::Hidden);
		DeadEndingWidgetUI->GI=GI;
		if (GI&&GI->GameSaveController)
		{
			DeadEndingWidgetUI->GameSaveController=GI->GameSaveController;
		}
	}
	
	MonolugueWidgetUI =CreateWidget<UMonolugueWidget>(GetWorld(),MonolugueWidgetFactory);
	if (MonolugueWidgetUI)
	{
		MonolugueWidgetUI->AddToViewport(1);
		MonolugueWidgetUI->SetVisibility(ESlateVisibility::Hidden);
	}
	
	LoadScreenUI=CreateWidget<ULoadScreenWidget>(GetWorld(),LoadScreenWidgetFactory);
	if (LoadScreenUI)
	{
		LoadScreenUI->AddToViewport(1);
		LoadScreenUI->SetVisibility(ESlateVisibility::Hidden);
		if (GI)
		{
			LoadScreenUI->LoadUpdateUI(GI->SaveSlotInfos);
		}
	}

	if (GI)
	{
		for (int i = 0; i < GI->ClueDataRowNames.Num(); i++)
		{
			// 겟수만큼 생성
			FClueData* ClueData =GI-> DT_Clue->FindRow<FClueData>(GI->ClueDataRowNames[i],TEXT(""));
			
			UClueSlotWidget* newSlot=CreateWidget<UClueSlotWidget>(GetWorld(),NoteUI->WBP_ClueInfo->ClueSlotWidgetFactory);
			
			 NoteUI->WBP_ClueInfo->ScrollBox_List->AddChild(newSlot);
			
			 NoteUI->WBP_ClueInfo->ClueSlots.Add(i,newSlot);
			
			 newSlot->SetTextClueName(ClueData->Name);
			 newSlot->Clue_ID=ClueData->ClueID;
			
			UE_LOG(LogTemp,Warning,TEXT("%s,%s"),*CALLINFO,*ClueData->Name);
			 if (ClueData->Had)
			 {
				 // 있으면
			 	newSlot->SetWidgetSwitcher(1);
			 	//newSlot->Btn_ClueSlot->SetIsEnabled(true);
			 	//newSlot->WidgetSwitcher->SetActiveWidgetIndex(1);
			 }
			 else
			 {
			 	// 없으면
			 	newSlot->WidgetSwitcher->SetActiveWidgetIndex(0);
			 }
			
		}

		// GI 의 NPCEventManage 받아서 로드하고 업데이트 하는 함수 
		UpdateNPCInfoWidget();
		// 진행하고 있던 퀘스트 목록을 받아서 로드하고 업데이트하기 
		InteractUI->LoadUpdateQuestSlot();
	}


	/*
	if (InventoryUI) //InventoryUI 초기화 작업
	{
		auto InvenSlotMap = InventoryUI->InvenSlots;
		//먼저 모든 행단위로 가져오기  

		for (int i = 0; i < GI->itemDataRowNames.Num(); i++)
		{
			// DT_itemData 에서 행 찾아서 
			FItemData* ItemData = GI->DT_itemData->FindRow<FItemData>(GI->itemDataRowNames[i] , TEXT(""));
			if (ItemData)
			{
				// 인벤토리 슬롯에 썸네일 이미지 할당
				InvenSlotMap[i]->Img_Thumnail->SetBrushFromTexture(ItemData->thumnail);
			}
		}
	}*/
	for (TActorIterator<APostProcessVolume> It(GetWorld(), APostProcessVolume::StaticClass()); It; ++It)
	{
		APostProcessVolume* PP=*It;

		if (PP->ActorHasTag(FName(TEXT("VignetteEffect"))))
		{
			AllPostProcessVolume=PP;
			// 로그 출력으로 확인하기
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Find AllPostProcessVolume")));
			
			//UMaterialInterface* BaseMaterial = Cast<UMaterialInterface>(AllPostProcessVolume->Settings.WeightedBlendables.Array[0].Object);
			
			if (BaseMaterial)
			{
				// 동적 인스턴스 생성
				PostProcessVignetteMatDynamic = UMaterialInstanceDynamic::Create(BaseMaterial, this);

				// 동적으로 넣어주는걸 마지막에 하기 
				//AllPostProcessVolume->Settings.WeightedBlendables.Array[0].Object = PostProcessVignetteMatDynamic;
			}
			
			if (PostProcessVignetteMatDynamic)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("PostProcessVignetteMatDynamic Exist")));
			}
		}
	}

	// 레벨로드시 비네트 효과
	PlayLevelLoadVignetteEffect();
}

void APlayerHUD::CreateYSEvance()
{
	if (!YSEvanceFactory)
		return;
	
	YsEvanceUserWidget = CreateWidget<UYSEvanceUI>(GetWorld(), YSEvanceFactory);
	
	if (YsEvanceUserWidget) {
		YsEvanceUserWidget->AddToViewport();
		YsEvanceUserWidget->SetVisibility(ESlateVisibility::Visible);
		//텍스트 애니메이션
		YsEvanceUserWidget->PlayAnim();
	}
}

void APlayerHUD::UpdateClueSlotWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateClueSlotWidget"));
	TArray<FName> RowNames = GI->DT_Clue->GetRowNames();

	for (int i = 0; i < GI->ClueDataRowNames.Num(); i++)
	{
		FClueData* ClueData =GI-> DT_Clue->FindRow<FClueData>(GI->ClueDataRowNames[i],TEXT(""));
		if (ClueData->Had)
		{
			UE_LOG(LogTemp,Warning,TEXT("%s,%s"),*CALLINFO,*ClueData->Name);
			UE_LOG(LogTemp,Warning,TEXT("%s,%d"),*CALLINFO,i);
			NoteUI->WBP_ClueInfo->ClueSlots[i]->SetWidgetSwitcher(1);
			NoteUI->WBP_ClueInfo->ClueSlots[i]->Btn_ClueSlot->SetIsEnabled(true);
			NoteUI->WBP_ClueInfo->ClueSlots[i]->WidgetSwitcher->SetActiveWidgetIndex(1);
		}
	}		
}

void APlayerHUD::UpdateNPCInfoWidget()
{
	// GI 의 TSet<FName> NPCEventManage; 을 보고 업데이트할꺼 만들어두기
	
	if (!GI->NPCEventManage.IsEmpty())
	{
		for (FName &EventName:GI->NPCEventManage)
		{
			FString EventString = EventName.ToString();
			NoteUI->WBP_NPCInfo->LoadUpdate(EventString);
		}
	}

	//도슨트 클리어했으면
	if (GI->ClearedNPC.Contains(2))
	{
		NoteUI->WBP_NPCInfo->WidgetSwitcher_Docent1->SetActiveWidgetIndex(1);
	}
	
}

void APlayerHUD::PlayDeadVignetteEffect()
{
	if (!PostProcessVignetteMatDynamic){return;}
	AllPostProcessVolume->Settings.WeightedBlendables.Array[0].Object = PostProcessVignetteMatDynamic;
	CurrentStrength = 0.0f;
	TargetStrength=200.0f;
	GetWorld()->GetTimerManager().SetTimer(
		VignetteTimerHandle,
		this,
		&APlayerHUD::UpdateVignetteStrength,
		TimerInterval,
		true
	);
	
}

void APlayerHUD::UpdateVignetteStrength()
{
	if (!PostProcessVignetteMatDynamic) return;

	CurrentStrength = FMath::Min(CurrentStrength + LerpStep, TargetStrength);
	PostProcessVignetteMatDynamic->SetScalarParameterValue("Vignette_Strength", CurrentStrength);
	
	if (CurrentStrength >= TargetStrength)
	{
		GetWorld()->GetTimerManager().ClearTimer(VignetteTimerHandle);
		AllPostProcessVolume->Settings.WeightedBlendables.Array[0].Object = nullptr;
	}
}

void APlayerHUD::PlayLevelLoadVignetteEffect()
{
	if (!PostProcessVignetteMatDynamic){return;}
	
	PostProcessVignetteMatDynamic->SetScalarParameterValue("Vignette_Strength", Levelload_CurrentStrength);
	AllPostProcessVolume->Settings.WeightedBlendables.Array[0].Object = PostProcessVignetteMatDynamic;
	GetWorld()->GetTimerManager().SetTimer(
		VignetteTimerHandle,
		this,
		&APlayerHUD::UpdateMinusVignetteStrength,
		TimerInterval,
		true
	);
}

void APlayerHUD::UpdateMinusVignetteStrength()
{
	if (!PostProcessVignetteMatDynamic) return;

	Levelload_CurrentStrength = FMath::Max(Levelload_CurrentStrength - Levelload_LerpStep, Levelload_TargetStrength);
	PostProcessVignetteMatDynamic->SetScalarParameterValue("Vignette_Strength", Levelload_CurrentStrength);
	
	if (Levelload_CurrentStrength <= Levelload_TargetStrength)
	{
		GetWorld()->GetTimerManager().ClearTimer(VignetteTimerHandle);
		AllPostProcessVolume->Settings.WeightedBlendables.Array[0].Object = nullptr;
	}
}
