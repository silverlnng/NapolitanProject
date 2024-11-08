// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"
#include "../YJ/NoteUI/InventoryWidget.h"
#include "../YJ/NoteUI/InvenSlotWidget.h"
#include "../YJ/PlayerHUD.h"
#include "Components/Image.h"
#include "../YJ/NoteUI/NoteWidget.h"

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();

	PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PlayerHUD=PC->GetHUD<APlayerHUD>();
	}

	if (!PlayerHUD){return;}

	/*FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle,[this]()
	{
		//////// 아이템 채우기 
		auto InvenSlotMap = PlayerHUD->NoteUI->WBP_Inventory->InvenSlots;
		
		DT_itemData = LoadObject<UDataTable>(nullptr ,TEXT("'/Game/YJ/Item/DT_Item.DT_Item'"));

		//먼저 모든 행단위로 가져오기  
		TArray<FName> RowNames = DT_itemData->GetRowNames();

		for (int i = 0; i < RowNames.Num(); i++)
		{
			FItemData* ItemData = DT_itemData->FindRow<FItemData>(RowNames[i] , TEXT(""));
			if (ItemData)
			{
				// 로그에 행 이름과 ItemName 출력
				//UE_LOG(LogTemp, Warning, TEXT("Row: %s, Item Name: %s"), *RowNames[i].ToString(), *ItemData->ItemInt);

				// 인벤토리 슬롯에 썸네일 이미지 할당

				InvenSlotMap[i]->Img_Thumnail->SetBrushFromTexture(ItemData->thumnail);
			}
			UE_LOG(LogTemp , Warning , TEXT("%s") , *RowNames[i].ToString());
			
		}
		 ///Script/Engine.DataTable'/Game/YJ/Item/DT_Souvenir.DT_Souvenir'
		//// 유물에 대한 데이터 SouvenirMap 채우기 
		DT_SouvenirData=LoadObject<UDataTable>(nullptr ,TEXT("'/Game/YJ/Item/DT_Souvenir.DT_Souvenir'"));

		//먼저 모든 행단위로 가져오기  
		TArray<FName> RowNames_ = DT_SouvenirData->GetRowNames();

		for (int i = 0; i < RowNames.Num(); i++)
		{
			FSouvenirData* SouvenirData = DT_SouvenirData->FindRow<FSouvenirData>(RowNames[i] , TEXT(""));
			if (SouvenirData)
			{
				// 로그에 행 이름과 ItemName 출력
				//UE_LOG(LogTemp, Warning, TEXT("Row: %s, Item Name: %s"), *RowNames[i].ToString(), *ItemData->ItemInt);

				// 인벤토리 슬롯에 썸네일 이미지 할당

				//InvenSlotMap[i]->Img_Thumnail->SetBrushFromTexture(ItemData->thumnail);
			}
			UE_LOG(LogTemp , Warning , TEXT("%s") , *RowNames[i].ToString());
			
		}
		
	},0.5f,false);*/
	
	
}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

