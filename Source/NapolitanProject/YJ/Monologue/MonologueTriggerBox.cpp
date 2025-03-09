// Fill out your copyright notice in the Description page of Project Settings.


#include "MonologueTriggerBox.h"

#include "MonolugueWidget.h"
#include "Components/BoxComponent.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
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
}

// Called when the game starts or when spawned
void AMonologueTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	TestPC = GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	if (TestPC)
	{
		MainCharacter =TestPC->GetPawn<ATestCharacter>();
		PlayerHUD=TestPC->GetHUD<APlayerHUD>();
	}
	BoxComp->OnComponentBeginOverlap.AddDynamic(this,&AMonologueTriggerBox::BoxCompBeginOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this,&AMonologueTriggerBox::BoxCompEndOverlap);

	CurrentIndex=0;

	/*DrawDebugBox(
	   GetWorld(),
	   BoxComp->GetComponentLocation(),
	   BoxComp->GetScaledBoxExtent(),
	   BoxComp->GetComponentRotation().Quaternion(),
	   FColor::Red,
	   true,  // 지속 시간
	   -1.0f, // 무한 지속
	   0,     // 두께
	   5.0f   // 선 두께
   );*/
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
		if (bHasTriggered)
		{
			return;
		}

		bHasTriggered = true; 
		// MainCharacter 의 MonologueUI 에 나오도록 해야함 .
		// 각자의 Monologue_ID 에 따라서 다른 글을 나오도록 하기

		PlayerHUD->MonolugueWidgetUI->SetOutputLines(TextLines);
		PlayerHUD->MonolugueWidgetUI->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMonologueTriggerBox::BoxCompEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

