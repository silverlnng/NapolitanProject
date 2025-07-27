// Fill out your copyright notice in the Description page of Project Settings.


#include "MonologueTriggerBox.h"

#include "MonolugueWidget.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/SaveGISubsystem.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

// Sets default values
AMonologueTriggerBox::AMonologueTriggerBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);
	BoxComp=CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComponent"));
	BoxComp->SetupAttachment(RootComponent);

	// 에디터 상에서만 보이는 아이콘
	EditorBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("EditorBillboard"));
	EditorBillboard->SetupAttachment(RootComponent);

	// 아이콘 이미지 설정 (Engine 기본 아이콘 사용)
	// /Script/Engine.Texture2D'/Engine/EditorResources/S_PortalActorIcon2.S_PortalActorIcon2'
	// /Script/Engine.Texture2D'/Game/YJ/BP/MonoloogueTriggerBox/MonologueIcon.MonologueIcon'
	static ConstructorHelpers::FObjectFinder<UTexture2D> IconTexture(TEXT("Texture2D'/Game/YJ/BP/MonoloogueTriggerBox/MonologueIcon.MonologueIcon'"));
	if (IconTexture.Succeeded())
	{
		EditorBillboard->SetSprite(IconTexture.Object);
	}
	
	FLinearColor LinearColor = FLinearColor(0.0f, 1.0f, 0.25f, 1.0f); // Example linear color
	FColor Color = LinearColor.ToFColor(true);
	BoxComp->ShapeColor=Color;
}

// Called when the game starts or when spawned
void AMonologueTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	
	GI = GetGameInstance<UMyTestGameInstance>();
	SaveGI = GI->GetSubsystem<USaveGISubsystem>();

	if (SaveGI->IsFromLoad)
	{
		if (SaveGI->ClearedNPC.Contains(GetAssociatedNpcID()))
		{
			Destroy();
		}
	}
	TestPC = GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	if (TestPC)
	{
		MainCharacter =TestPC->GetPawn<ATestCharacter>();
		PlayerHUD=TestPC->GetHUD<APlayerHUD>();
	}
	BoxComp->OnComponentBeginOverlap.AddDynamic(this,&AMonologueTriggerBox::BoxCompBeginOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this,&AMonologueTriggerBox::BoxCompEndOverlap);

	CurrentIndex=0;
}

// Called every frame
void AMonologueTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMonologueTriggerBox::BoxCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor->IsA(ATestCharacter::StaticClass()))
	{
		if (OnlyOnce&&bHasTriggered)
		{
			return;
		}

		bHasTriggered = true; 
		// MainCharacter 의 MonologueUI 에 나오도록 해야함 .
		// 각자의 Monologue_ID 에 따라서 다른 글을 나오도록 하기
		PlayerHUD->MonolugueWidgetUI->SetOutputLines(TextLines);
		PlayerHUD->MonolugueWidgetUI->SetVisibility(ESlateVisibility::Visible);

		// 움직임을 잠시 멈추기
		// bIsBeingAttacked
		if (MainCharacter)
		MainCharacter->bIsBeingAttacked=true;
		// 다시 움직이게 만들 타이머
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AMonologueTriggerBox::MoveCharacter, MoveStopTime, false);
		
	}
}

void AMonologueTriggerBox::BoxCompEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AMonologueTriggerBox::MoveCharacter()
{
	if (MainCharacter)
	{
		MainCharacter->bIsBeingAttacked=false;
	}
}

