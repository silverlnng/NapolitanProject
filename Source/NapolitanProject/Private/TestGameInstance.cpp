// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameInstance.h"

void UTestGameInstance::Init()
{
	Super::Init();
	DT_itemData = LoadObject<UDataTable>(nullptr ,TEXT("'/Game/YJ/Item/DT_Item.DT_Item'"));
	itemDataRowNames = DT_itemData->GetRowNames();
	
	DT_SouvenirData=LoadObject<UDataTable>(nullptr ,TEXT("'/Game/YJ/Item/DT_Souvenir.DT_Souvenir'"));
	SouvenirDataRowNames=DT_SouvenirData->GetRowNames();
}
