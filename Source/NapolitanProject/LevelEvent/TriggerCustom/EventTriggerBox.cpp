// Fill out your copyright notice in the Description page of Project Settings.


#include "EventTriggerBox.h"

#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

// Sets default values
AEventTriggerBox::AEventTriggerBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);

	BoxComp=CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComp->SetupAttachment(RootComponent);

	// 에디터 상에서만 보이는 아이콘
	EditorBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("EditorBillboard"));
	EditorBillboard->SetupAttachment(RootComponent);

	// 아이콘 이미지 설정 (Engine 기본 아이콘 사용)
	// /Script/Engine.Texture2D'/Engine/EditorResources/S_PortalActorIcon2.S_PortalActorIcon2'
	static ConstructorHelpers::FObjectFinder<UTexture2D> IconTexture(TEXT("Texture2D'/Engine/EditorResources/S_PortalActorIcon2.S_PortalActorIcon2'"));
	if (IconTexture.Succeeded())
	{
		EditorBillboard->SetSprite(IconTexture.Object);
	}
}

// Called when the game starts or when spawned
void AEventTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	TestPC=GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	if (TestPC)
	{
		MainCharacter =TestPC->GetPawn<ATestCharacter>();
		PlayerHUD=TestPC->GetHUD<APlayerHUD>();
	}

	//BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AEventTriggerBox::BeginOverlap);

	GI=GetGameInstance<UMyTestGameInstance>();
}

// Called every frame
void AEventTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEventTriggerBox::BindBeginOverlap()
{
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AEventTriggerBox::BeginOverlap);
}

void AEventTriggerBox::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AEventTriggerBox::BindEndOverlap()
{
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AEventTriggerBox::EndOverlap);
}

void AEventTriggerBox::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}



