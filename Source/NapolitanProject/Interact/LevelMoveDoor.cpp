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
	GI->SetLevelMoveToDoor(true); //저장된 위치를 사용하라는 bool 변수 설정 함수
			
	// + 위치 적용 플래그 설정
	if (bMoveFromLobby) // 지금 현재레벨이 로비일때만 게임저장
	{
		//  지금 현재레벨이 로비일때만 이동하기전 위치를 저장해두기!!!!!!
		if (MainCharacter)
		{
			MainCharacter->SaveTransform=FromLevelLocComp->GetComponentTransform();
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("MyVector: %s"), *MainCharacter->SaveTransform.GetLocation().ToString()));
		}
		GI->SavePlayerFTransform(FromLevelLocComp->GetComponentTransform());
		GI->GameSaveController->SaveGameToSlot(3); //여긴 자동저장 같은 느낌
				
		GI->AsyncLoadLoadLevel(MoveToLevel); //레벨 이동할때 로딩이 되도록

		//레벨 이동하기 전에 타이머를 종료하고 가야 에러가 안남 !!!!!!!!
		if (GetWorld()->GetTimerManager().IsTimerActive(PlayerHUD->MonolugueWidgetUI->TextUpdateTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(PlayerHUD->MonolugueWidgetUI->TextUpdateTimerHandle);
		}
		if (GetWorld()->GetTimerManager().IsTimerActive(PlayerHUD->MonolugueWidgetUI->FinalTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(PlayerHUD->MonolugueWidgetUI->FinalTimerHandle);
		}
			
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			UGameplayStatics::OpenLevelBySoftObjectPtr(this,MoveToLevel,true); //레벨 변경
		}, 1.0f, false);
	}
	else // 다른레벨에서 다시 로비로 돌아갈때 사용하는 것. 
	{
		GI->GameSaveController->LoadGameFromSlot(4);
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




