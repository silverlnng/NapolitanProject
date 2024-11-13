// Fill out your copyright notice in the Description page of Project Settings.


#include "ClueInfoWidget.h"

#include "Components/RichTextBlock.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"

void UClueInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
	GI =GetGameInstance<UMyTestGameInstance>();
}

void UClueInfoWidget::SetClueContent(int32 Clue_ID)
{
	// dt 에서
	//RichText_ClueName->SetText()
	FName Clue_FName = FName(*FString::FromInt(Clue_ID));
	if (!GI->DT_Clue){return;}
	FClueData* ClueData= GI->DT_Clue->FindRow<FClueData>(Clue_FName , TEXT(""));
	if (!ClueData){ UE_LOG(LogTemp,Warning,TEXT("ClueData null")) return;}
	
	FString name = ClueData->Name;
	RichText_ClueName->SetText(FText::FromString(name));

	FString content = ClueData->Content;

	RichText_ClueInfo->SetText(FText::FromString(content));
}
