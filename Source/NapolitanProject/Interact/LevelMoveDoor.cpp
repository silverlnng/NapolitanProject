// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelMoveDoor.h"

#include "Components/BoxComponent.h"
#include "Components/SlateWrapperTypes.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/YJ/Monologue/MonolugueWidget.h"

// Sets default values
ALevelMoveDoor::ALevelMoveDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);

	BoxComp=CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComp->SetupAttachment(RootComponent);

	
	BoxComp2=CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent2"));
	BoxComp2->SetupAttachment(RootComponent);
	
	StaticMeshComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComp->SetupAttachment(BoxComp);
}

// Called when the game starts or when spawned
void ALevelMoveDoor::BeginPlay()
{
	Super::BeginPlay();
	
	TestPC=GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	if (TestPC)
	{
		MainCharacter =TestPC->GetPawn<ATestCharacter>();
		PlayerHUD=TestPC->GetHUD<APlayerHUD>();
	}
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ALevelMoveDoor::BeginOverlap);

	GI=GetGameInstance<UMyTestGameInstance>();
	
	// 콜리전 설정은 폰 만되도록 블프에서 설정하기
	
}

// Called every frame
void ALevelMoveDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelMoveDoor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 그림 앞에 한번들어오면 UI 으로 나옴
	UE_LOG(LogTemp, Warning, TEXT("문과 충돌"));
	if(OtherActor)
	{
		auto* cha=Cast<ATestCharacter>(OtherActor);
		if(cha)
		{
			PlayerHUD->MonolugueWidgetUI->SetOutputLines(TextLines);
			PlayerHUD->MonolugueWidgetUI->SetVisibility(ESlateVisibility::Visible);
			BoxComp2->OnComponentBeginOverlap.AddDynamic(this, &ALevelMoveDoor::BeginOverlap2);
		}
	}
}

// 더 작은 범위의 트리거 박스
void ALevelMoveDoor::BeginOverlap2(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 이때 레벨 무브
	if(OtherActor)
	{
		auto* cha=Cast<ATestCharacter>(OtherActor);
		if(cha)
		{

			// 이동하기전 위치를 저장해두기
			
			// 타이머 돌리고 
			GI->AsyncLoadLoadLevel(MoveToLevel);
			
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				UGameplayStatics::OpenLevelBySoftObjectPtr(this,MoveToLevel,true); //레벨 변경
			}, 2.0f, false);
		}	
	}
	
}




