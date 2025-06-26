// Fill out your copyright notice in the Description page of Project Settings.


#include "EventTriggerBox.h"

#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
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
	// /Script/Engine.Texture2D'/Game/YJ/BP/LevelTrigger/Bounding_Headline.Bounding_Headline'
	static ConstructorHelpers::FObjectFinder<UTexture2D> IconTexture(TEXT("Texture2D'/Game/YJ/BP/LevelTrigger/Bounding_Headline.Bounding_Headline'"));
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

void AEventTriggerBox::loadSublevel()
{
	UUIDCounter=0;
	GetWorld()->GetTimerManager().SetTimer(LoadSubLevelTimerHandle, [this]()
	{
		for (auto subLevel:LoadSubLevelArray)
		{
			FLatentActionInfo LatentAction;
			LatentAction.CallbackTarget = this;
			LatentAction.UUID = UUIDCounter++;
			LatentAction.Linkage = 0;
			LatentAction.ExecutionFunction = NAME_None;
			UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(),subLevel,true,true,LatentAction);

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("LoadedGame_SubLevelArray")));
		}
			
	}, 1.0f, false);
	
}

void AEventTriggerBox::UnloadSublevel()
{
	UUIDCounter=0;
	GetWorld()->GetTimerManager().SetTimer(UnLoadSubLevelTimerHandle, [this]()
	{
		for (auto subLevel:UnLoadSubLevelArray)
		{
			FLatentActionInfo LatentAction;
			LatentAction.CallbackTarget = this;
			LatentAction.UUID = UUIDCounter++;
			LatentAction.Linkage = 0;
			LatentAction.ExecutionFunction = NAME_None;
			UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(GetWorld(),subLevel,LatentAction,true);

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("UnLoadedGame_SubLevelArray")));
		}
			
	}, 1.0f, false);
}

void AEventTriggerBox::ProcessNextSubLevel()
{
	if (CurrentIndex < LoadSubLevelArray.Num())
	{
		FLatentActionInfo LatentAction;
		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(),LoadSubLevelArray[CurrentIndex],true,true,LatentAction);
		
		CurrentIndex++;

		// 다음 아이템을 1초 후 처리
		GetWorldTimerManager().SetTimer(LoadSubLevelTimerHandle, this, &AEventTriggerBox::ProcessNextSubLevel, 1.5f, false);
	}
	else
	{
		// 종료 처리
		UE_LOG(LogTemp, Log, TEXT("모든 레벨 로드 완료"));
	}
}



