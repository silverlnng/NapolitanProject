// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetForItem_LobbyDoor.h"
#include "EngineUtils.h"
#include "ExitDoor_Lobby.h"
#include "NapolitanProject/NapolitanProject.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/YJ/Monologue/MonolugueWidget.h"

void ATargetForItem_LobbyDoor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (bisDissolve && DynamicMaterial)
	{
		dissolveAnimValue += DeltaSeconds / 4;

		// 원하는 범위 (0.5에서 -0.5)로 클램핑
		float DissolveValue = FMath::Clamp(0.5f - dissolveAnimValue, -0.5f, 0.5f);
		DynamicMaterial->SetScalarParameterValue(TEXT("dissolve"), DissolveValue);

		if (DissolveValue <= -0.5f)
		{
			bisDissolve = false;
			StaticMeshComp->SetVisibility(false);
		}
	}
}

void ATargetForItem_LobbyDoor::CheckItem(class AItemActor* curItem)
{
	Super::CheckItem(curItem);
}

void ATargetForItem_LobbyDoor::CheckItemSuccess()
{
	Super::CheckItemSuccess();
	UE_LOG(LogTemp, Error, TEXT("%s CheckItemSuccess"),*CALLINFO);
	//문이 열리도록 만들어야함.

	// 캐릭터와 충돌안되도록
	StaticMeshComp->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	
	// 
	DissolveEvent();

	// 로비도어 문을 열리도록 .
	// 도어중에 tag 달린거로 .
	AExitDoor_Lobby* Door=nullptr;
	for (TActorIterator<AExitDoor_Lobby> It(GetWorld(), AExitDoor_Lobby::StaticClass()); It; ++It)
	{
		Door = *It;
	}
	if (Door)
	{
		Door->StartRotateDoor();
	}
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



void ATargetForItem_LobbyDoor::DissolveEvent()
{
	if (DissolveMaterial)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(DissolveMaterial, this);
		if (DynamicMaterial)
		{
			StaticMeshComp->SetMaterial(0, DynamicMaterial);
		}
	}

	bisDissolve = true;
}
