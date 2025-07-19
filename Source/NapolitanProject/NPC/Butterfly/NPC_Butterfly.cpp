// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Butterfly.h"

#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/Interact/ItemActor.h"
#include "NapolitanProject/Interact/TargetForItem_BurgerPlate.h"

void ANPC_Butterfly::BeginPlay()
{
	Super::BeginPlay();
}

void ANPC_Butterfly::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

int32 ANPC_Butterfly::GetNPCID()
{
	return NPC_ID;
}

int32 ANPC_Butterfly::GetState()
{
	return State;
}

void ANPC_Butterfly::Interact()
{
	Super::Interact();
}

void ANPC_Butterfly::ResultEvent(int32 result)
{
	if (1==State)
	{
		if (0==result)
		{
			//State=2;
			//TestPC->StartEndNPCDialougue(true);
			//TestPC->SetNPCDialougueText(0);
		}
	}
	else if (3==State)
	{
		if (9==result)
		{
			//SpawnItems();
		}
	}
}

void ANPC_Butterfly::SpawnItems()
{
	FTransform SpawnTransform;
	// 발끝 위치를 기준으로 스폰 위치 설정
	if (TargetForItem_BurgerPlate)
	{
		//FVector BurgerPlateLocation = TargetForItem_BurgerPlate->SceneComp3->;
		SpawnTransform=TargetForItem_BurgerPlate->SceneComp3->GetComponentTransform();
	}
	//
	if (CutterItemClass)
	{
		AActor* CutterItem = GetWorld()->SpawnActor<AItemActor>(CutterItemClass, SpawnTransform );
	}

	// ui 닫도록 하기
	
	
}

void ANPC_Butterfly::PlayWingMontage()
{
	GetMesh()->PlayAnimation(WingMontage,true);
}
