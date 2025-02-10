// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetForItem_LobbyDoor.h"

#include "NapolitanProject/NapolitanProject.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/YJ/Monologue/MonolugueWidget.h"

void ATargetForItem_LobbyDoor::CheckItem(class AItemActor* curItem)
{
	Super::CheckItem(curItem);
}

void ATargetForItem_LobbyDoor::CheckItemSuccess()
{
	//Super::CheckItemSuccess();
	UE_LOG(LogTemp, Error, TEXT("%s CheckItemSuccess"),*CALLINFO);
	//문이 열리도록 만들어야함.

	//
}

void ATargetForItem_LobbyDoor::CheckItemFail()
{
	//Super::CheckItemFail();
	UE_LOG(LogTemp, Error, TEXT("%s CheckItemFail"),*CALLINFO);
	FString CheckItemFail =FString(TEXT("<Monologue>이걸로는 열수없다"));
	PlayerHUD->MonolugueWidgetUI->SetVisibility(ESlateVisibility::Visible);
	PlayerHUD->MonolugueWidgetUI->SetText_Dialogue(CheckItemFail);
}

void ATargetForItem_LobbyDoor::NoItem()
{
	//Super::NoItem();
	UE_LOG(LogTemp, Error, TEXT("%s NoItem"),*CALLINFO);
	FString NoItem =FString(TEXT("<Monologue>자물쇠로 굳게 닫힌 문이다"));
	PlayerHUD->MonolugueWidgetUI->SetVisibility(ESlateVisibility::Visible);
	PlayerHUD->MonolugueWidgetUI->SetText_Dialogue(NoItem);
}
