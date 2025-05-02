// Fill out your copyright notice in the Description page of Project Settings.


#include "Door_2Floor.h"

#include "Components/BoxComponent.h"
#include "Components/SlateWrapperTypes.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/YJ/Monologue/MonolugueWidget.h"

void ADoor_2Floor::BeginPlay()
{
	Super::BeginPlay();
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ADoor_2Floor::BeginOverlap);
	
	//GI=GetGameInstance<UMyTestGameInstance>();
	// 청소부 만났으면 바인드해제하기
	// QuestSlots
	
	// 게임로드할때 사용 "머리를 찾아주기" , 또는 ""
	FTimerHandle LoadTimer;
	GetWorld()->GetTimerManager().SetTimer(LoadTimer,[this]()
	{
		FString QuestText =FString(TEXT("머리를 찾아주기"));
		FString QuestText2 =FString(TEXT("머리를 가져다 주자"));
		
		if (GI&&(GI->QuestSlots.Contains(QuestText)||GI->QuestSlots.Contains(QuestText2)))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("QuestSlots.Contains머리를 찾아주기")));
			BoxComp->OnComponentBeginOverlap.Clear();
			JustRotateDoor();
		}
	},0.5f,false);
	
}


void ADoor_2Floor::UnBindBeginOverlap()
{
	//청소부 만나고 나면 해제하기
	BoxComp->OnComponentBeginOverlap.Clear();
	// 문열리도록 하기 
	StartRotateDoor();
}

void ADoor_2Floor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Super::OnBeginOverlap(OverlappedComponent , OtherActor , OtherComp , OtherBodyIndex , bFromSweep , SweepResult);
	// 독백대사 나오도록 하기
	if(OtherActor)
	{
		
		auto* Player = Cast<ATestCharacter>(OtherActor);
		if (Player)
		//퀘스트 있는지 검사
		{
			FString NoEnter =FString(TEXT("<Monologue>2층 전시관은 닫혀있어서 갈수없다"));
			Player->PlayerHUD->MonolugueWidgetUI->SetVisibility(ESlateVisibility::Visible);
			Player->PlayerHUD->MonolugueWidgetUI->SetText_Dialogue(NoEnter);
		}
	}
}

void ADoor_2Floor::RotateDoor()
{
	Super::RotateDoor();
}

void ADoor_2Floor::RotateDoor2()
{
	Super::RotateDoor2();
}

void ADoor_2Floor::StartRotateDoor()
{
	if(bIsOpenDoor) // 한번만 열리도록 만든 것
		return;

	bIsOpenDoor = true;
	if (OpenDoorSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld() , OpenDoorSound); // 소리 재생
	}

	// 목표 Yaw 계산 (현재 Yaw에서 90도 추가)
	FRotator CurrentRotation = ExitDoor->GetRelativeRotation();
	TargetYaw = CurrentRotation.Yaw + 90.0f;
	
	TargetYaw2 = CurrentRotation.Yaw - 90.0f;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle , this , &ADoor_2Floor::RotateDoor , 0.01f , true);
	

	GetWorld()->GetTimerManager().SetTimer(TimerHandle2 , this , &ADoor_2Floor::RotateDoor2 , 0.01f , true);
}

void ADoor_2Floor::JustRotateDoor()
{
	FRotator CurrentRotation2 = ExitDoor2->GetRelativeRotation();
	TargetYaw2 = CurrentRotation2.Yaw - 90.0f;
	ExitDoor2->SetRelativeRotation(FRotator(CurrentRotation2.Pitch, TargetYaw2, CurrentRotation2.Roll));
	
	FRotator CurrentRotation = ExitDoor->GetRelativeRotation();
	TargetYaw = CurrentRotation.Yaw + 90.0f;
	ExitDoor->SetRelativeRotation(FRotator(CurrentRotation.Pitch, TargetYaw, CurrentRotation.Roll)); 
}
