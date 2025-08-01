// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Butterfly.h"

#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "NapolitanProject/GameFrameWork/SaveGISubsystem.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/Interact/Item/ItemActor.h"
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
	if (WingMontage)
	{
		GetMesh()->PlayAnimation(WingMontage,true);
	}
}

void ANPC_Butterfly::PlayEatMontage()
{
	if (EatMontage)
	{
		GetMesh()->PlayAnimation(EatMontage,false);
	}
}

void ANPC_Butterfly::PlayFlyHighMontage()
{
	if (FlyHighMontage)
	{
		GetMesh()->PlayAnimation(FlyHighMontage,false);
	}
}

void ANPC_Butterfly::Cleared()
{
	GetComponentByClass<UCapsuleComponent>()->SetCollisionProfileName(FName("ClearedNPC"));
	SaveGI->ClearedNPC.Add(GetNPCID());
	FString id =FString::FromInt(GetNPCID());
}

void ANPC_Butterfly::CloseUpCam()
{
	UE_LOG(LogTemp, Log, TEXT("🛑ADocentV2::CloseUPCam"));

	PlayWingMontage();
	
	this->AttachToComponent(MainCharacter->CenterArrowComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	// Arrow를 바라보도록 회전 보간
	FRotator DesiredRot = (MainCharacter->GetActorLocation() - this->GetActorLocation()).Rotation();
	
	this->SetActorRotation(DesiredRot);
}
