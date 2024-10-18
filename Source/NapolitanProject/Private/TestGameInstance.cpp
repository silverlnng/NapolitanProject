// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameInstance.h"

void UTestGameInstance::Init()
{
	Super::Init();
	DT_itemData = LoadObject<UDataTable>(nullptr ,TEXT("'/Game/YJ/Item/DT_Item.DT_Item'"));
	itemDataRowNames = DT_itemData->GetRowNames();
	
	DT_SouvenirData=LoadObject<UDataTable>(nullptr ,TEXT("'/Game/YJ/Item/DT_Souvenir.DT_Souvenir'"));
	SouvenirDataRowNames=DT_SouvenirData->GetRowNames();

	// 언어선택 기본값 한글 :0 ,영어:1
	lang=0;
}

void UTestGameInstance::SetGameInstanceLang(int32 value)
{
	lang=value;
}

int32 UTestGameInstance::GetGameInstanceLang()
{
	return lang;
}
