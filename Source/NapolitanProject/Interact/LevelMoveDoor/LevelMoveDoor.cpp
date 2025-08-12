// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelMoveDoor.h"
#include "Components/BoxComponent.h"
#include "Components/SlateWrapperTypes.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/SaveGISubsystem.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/YJ/Monologue/MonolugueWidget.h"
#include "NapolitanProject/YJ/Save/GameSaveController.h"

// Sets default values
ALevelMoveDoor::ALevelMoveDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);

	BoxComp=CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComp->SetupAttachment(RootComponent);

	
	//BoxComp2=CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent2"));
	//BoxComp2->SetupAttachment(RootComponent);
	
	StaticMeshComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComp->SetupAttachment(BoxComp);

	FromLevelLocComp =CreateDefaultSubobject<USceneComponent>(TEXT("FromLevelLoc"));
	FromLevelLocComp->SetupAttachment(RootComponent);
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
	SaveGI = GI->GetSubsystem<USaveGISubsystem>();
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
		MainCharacter=cha;
		if(cha)
		{
			
			if (!TextLines.IsEmpty())
			{
				// 나올 독백이 있으면 끝내고 레벨 이동하기 
				PlayerHUD->MonolugueWidgetUI->OnTimerFinished.AddDynamic(this, &ALevelMoveDoor::LevelMove);
			
				PlayerHUD->MonolugueWidgetUI->SetOutputLines(TextLines);
				PlayerHUD->MonolugueWidgetUI->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				LevelMove();
			}
			
		}
	}
}

void ALevelMoveDoor::LevelMove()
{
	SaveGI->SetLevelMoveToDoor(true); //저장된 위치를 사용하라는 bool 변수 설정 함수
			
	// + 위치 적용 플래그 설정
	if (bMoveFromLobby) // 지금 현재레벨이 로비일때만 게임저장= 로비에서 다른레벨로 넘어갈때 실행하는것
	{
		//  지금 현재레벨이 로비일때만 이동하기전 위치를 저장해두기!!!!!!
		if (MainCharacter)
		{
			MainCharacter->SaveTransform=FromLevelLocComp->GetComponentTransform();
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("MyVector: %s"), *MainCharacter->SaveTransform.GetLocation().ToString()));
		}
		SaveGI->SavePlayerFTransform(FromLevelLocComp->GetComponentTransform());

		// 로드해야하는 주위 서브레벨들
		if (!SaveGI->SubLevelArray.IsEmpty())
		{
			SaveGI->SubLevelArray.Empty();
		}
		
		for (auto Level:SubLevelArray)
		{
			SaveGI->SubLevelArray.Add(Level);
		}
		
		SaveGI->GameSaveController->SaveGameToSlot(3); //여긴 자동저장 . 슬롯 3에 이전과 동일하도록 저장을 함 
		
		//레벨 이동하기 전에 타이머를 종료하고 가야 에러가 안남 !!!!!!!!
		if (GetWorld()->GetTimerManager().IsTimerActive(PlayerHUD->MonolugueWidgetUI->TextUpdateTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(PlayerHUD->MonolugueWidgetUI->TextUpdateTimerHandle);
		}
		if (GetWorld()->GetTimerManager().IsTimerActive(PlayerHUD->MonolugueWidgetUI->FinalTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(PlayerHUD->MonolugueWidgetUI->FinalTimerHandle);
		}
			
		//GI->AsyncLoadLoadLevel(MoveToLevel); //레벨 이동할때 로딩이 되도록
		// GI->PreloadLevel(MoveToLevelPathString);
		// 그림으로 레벨이동은 preload 를 안하고 해보기 
		
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			UGameplayStatics::OpenLevelBySoftObjectPtr(this,MoveToLevel,true); //레벨 변경
		}, 1.f, false);
	}
	else // 다른레벨에서 다시 로비로 돌아갈때 사용하는 것. 
	{
		FString SlotName = TEXT("SaveSlot_3");
		UTestSaveGame* LoadedGame = Cast<UTestSaveGame>(
			UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	
		if (LoadedGame)
		{
			// 거미 방에서만 실행시켜도 되는 거 //
			LoadedGame->SavedItems=SaveGI->SavedItems;
			LoadedGame->CatchSpiderNum=SaveGI->CatchSpiderNum;
			UGameplayStatics::SaveGameToSlot(LoadedGame , SlotName , 0);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("SaveSlot_3 LoadedGame Null")));
		}
		
		SaveGI->GameSaveController->LoadGameFromSlot(3);
		// 여기에 오픈레벨(로비) 가 있음
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
		}	
	}
	
}




