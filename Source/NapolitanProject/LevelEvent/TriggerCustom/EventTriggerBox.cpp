// Fill out your copyright notice in the Description page of Project Settings.


#include "EventTriggerBox.h"

#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
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

	
	TriggerLeft = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerLeft"));
	TriggerLeft->SetupAttachment(RootComponent);
	TriggerLeft->SetCollisionProfileName(FName(TEXT("Trigger")));
	TriggerLeft->SetBoxExtent(FVector(390, 30, 250));
	TriggerLeft->SetRelativeLocation(FVector(0, -40, 0)); 

	FLinearColor LinearColor = FLinearColor(0.96664f, 0.0f, 1.0f, 1.0f); 
	FColor Color = LinearColor.ToFColor(true);
	TriggerLeft->ShapeColor=Color;

	
	TriggerRight = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerRight"));
	TriggerRight->SetupAttachment(RootComponent);
	TriggerRight->SetCollisionProfileName(FName(TEXT("Trigger")));
	TriggerRight->SetBoxExtent(FVector(390, 30, 250));
	TriggerRight->SetRelativeLocation(FVector(0, 40, 0)); 
	
	FLinearColor LinearColor2 = FLinearColor(0.025808, 0.0f, 1.0f, 1.0f); 
	FColor Color2 = LinearColor2.ToFColor(true);
	TriggerRight->ShapeColor=Color2;
	
	// 에디터 상에서만 보이는 아이콘
	EditorBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("EditorBillboard"));
	EditorBillboard->SetupAttachment(RootComponent);

	MessageWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("MessageWidget"));
	MessageWidget->SetupAttachment(RootComponent);
	MessageWidget->SetWidgetSpace(EWidgetSpace::World);
	MessageWidget->SetDrawSize(FVector2D(300, 100));
	MessageWidget->SetRelativeLocation(FVector(0, 0, 200));
	MessageWidget->SetVisibility(false); // 처음엔 숨김

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
	GI=GetGameInstance<UMyTestGameInstance>();
	
	BindBeginOverlap();
}

// Called every frame
void AEventTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEventTriggerBox::BindBeginOverlap()
{
	TriggerLeft->OnComponentBeginOverlap.AddDynamic(this, &AEventTriggerBox::BeginOverlap);
	TriggerRight->OnComponentBeginOverlap.AddDynamic(this, &AEventTriggerBox::BeginOverlap);
}

void AEventTriggerBox::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || !OtherActor->IsA(ATestCharacter::StaticClass())) return;
	
	if (!OtherComp->IsA<UCapsuleComponent>())return;
	
	ETriggerSide ThisSide = ETriggerSide::None;

	if (OverlappedComp == TriggerLeft)
	{
		ThisSide = ETriggerSide::Left;
		UE_LOG(LogTemp, Log, TEXT("TriggerLeft"));
	}
	else if (OverlappedComp == TriggerRight)
	{
		ThisSide = ETriggerSide::Right;
		UE_LOG(LogTemp, Log, TEXT("TriggerRight"));
	}

	if (ThisSide == ETriggerSide::None) return;

	if (!FirstTriggerMap.Contains(OtherActor))
	{
		// 첫 진입
		FirstTriggerMap.Add(OtherActor, ThisSide);
	}
	else
	{
		// 두 번째 진입
		ETriggerSide FirstSide = FirstTriggerMap[OtherActor];
		FirstTriggerMap.Remove(OtherActor);

		HandleDirection(OtherActor, FirstSide, ThisSide);
	}
}

void AEventTriggerBox::HandleDirection(AActor* PlayerActor, ETriggerSide FirstSide, ETriggerSide SecondSide)
{
	
	if (FirstSide == ETriggerSide::Left && SecondSide == ETriggerSide::Right)
	{
		UE_LOG(LogTemp, Log, TEXT("방향: 왼쪽 → 오른쪽"));
		// TODO: Level Load 등
		WhenLToR();
	}
	else if (FirstSide == ETriggerSide::Right && SecondSide == ETriggerSide::Left)
	{
		UE_LOG(LogTemp, Log, TEXT("방향: 오른쪽 → 왼쪽"));
		// TODO: Level Unload 등
		WhenRToL();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("같은 방향 두 번 트리거됨 또는 알 수 없음"));
	}
}


void AEventTriggerBox::BindEndOverlap()
{
	TriggerLeft->OnComponentEndOverlap.AddDynamic(this, &AEventTriggerBox::EndOverlap);
	TriggerRight->OnComponentEndOverlap.AddDynamic(this, &AEventTriggerBox::EndOverlap);
}

void AEventTriggerBox::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void AEventTriggerBox::WhenLToR()
{
	if (!LToR_LoadLevelArray.IsEmpty())
	{
		LoadUUIDCounter = 0;

		for (auto subLevel : LToR_LoadLevelArray)
		{
			FLatentActionInfo LatentAction;
			LatentAction.CallbackTarget = this;
			LatentAction.UUID = LoadUUIDCounter++;
			LatentAction.Linkage = 0;
			LatentAction.ExecutionFunction = NAME_None;
			UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld() , subLevel , true , true , LatentAction);

			GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Green ,
			                                 FString::Printf(TEXT("LoadedGame_SubLevelArray")));
		}
	}
	
	if (!LToR_UnLoadLevelArray.IsEmpty())
	{
		GetWorld()->GetTimerManager().SetTimer(UnLoadSubLevelTimerHandle , [this]()
		{
			UnLoadUUIDCounter=0;
			UnLoadLevel(&LToR_UnLoadLevelArray);
		} , 1.0f , false);
	}
	
}

void AEventTriggerBox::WhenRToL()
{
	if (!RToL_LoadLevelArray.IsEmpty())
	{
		LoadUUIDCounter=0;
		for (auto subLevel : RToL_LoadLevelArray)
		{
			FLatentActionInfo LatentAction;
			LatentAction.CallbackTarget = this;
			LatentAction.UUID = LoadUUIDCounter++;
			LatentAction.Linkage = 0;
			LatentAction.ExecutionFunction = NAME_None;
			UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld() , subLevel , true , true , LatentAction);

			GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Green ,
			                                 FString::Printf(TEXT("LoadedGame_SubLevelArray")));
		}
	}
	
	if (!RToL_UnLoadLevelArray.IsEmpty())
	{
		GetWorld()->GetTimerManager().SetTimer(UnLoadSubLevelTimerHandle , [this]()
		{
			UnLoadUUIDCounter=0;
			UnLoadLevel(&RToL_UnLoadLevelArray);
		} , 1.0f , false);
	}

}

void AEventTriggerBox::UnLoadLevel(TArray<TSoftObjectPtr<UWorld>>* UnLoadLevelArray)
{
	for (auto subLevel : *UnLoadLevelArray)
	{
		FLatentActionInfo LatentAction;
		LatentAction.CallbackTarget = this;
		LatentAction.UUID = UnLoadUUIDCounter++;
		LatentAction.Linkage = 0;
		LatentAction.ExecutionFunction = NAME_None;
		UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(GetWorld() , subLevel , LatentAction , true);

		GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Green ,
										 FString::Printf(TEXT("UnLoadedGame_SubLevelArray")));
	}
}


void AEventTriggerBox::ProcessNextSubLevel()
{
	if (CurrentIndex < LToR_LoadLevelArray.Num())
	{
		FLatentActionInfo LatentAction;
		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(),LToR_LoadLevelArray[CurrentIndex],true,true,LatentAction);
		
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



