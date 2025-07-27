// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetForItem.h"

#include "Item/ItemActor.h"
#include "Components/BoxComponent.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

// Sets default values
ATargetForItem::ATargetForItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);

	BoxComp=CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComp->SetupAttachment(RootComponent);

	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,ECR_Block);

	StaticMeshComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComp->SetupAttachment(BoxComp);

}

// Called when the game starts or when spawned
void ATargetForItem::BeginPlay()
{
	Super::BeginPlay();
	TestPC=GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	if (TestPC)
	{
		MainCharacter =TestPC->GetPawn<ATestCharacter>();
		PlayerHUD=TestPC->GetHUD<APlayerHUD>();
	}
}

// Called every frame
void ATargetForItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATargetForItem::CheckItem(class AItemActor* curItem)
{
	// curItem 을 검증해서 작동되도록 만들기
	if (curItem->ItemID==TargetItemID)
	{
		CheckItemSuccess();
	}
	else
	{
		// 아이템은 있지만 나와 작용하는 아이템이 아님 .
		CheckItemFail();
	}
	
}

void ATargetForItem::CheckItemSuccess()
{
	// 성공하면 다시 interact할필요없어서. 
	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,ECR_Ignore);
}

void ATargetForItem::CheckItemFail()
{
}

void ATargetForItem::NoItem()
{
}

void ATargetForItem::IInteract()
{
	if (MainCharacter->curItem)
	{
		CheckItem(MainCharacter->curItem);
	}
	else if (!MainCharacter->curItem)
	{
		NoItem();
	}
}
