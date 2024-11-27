// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitDoor_First.h"

#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/BDUI/NormalGameEndingWidget.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"


void AExitDoor_First::BindBeginOverlap()
{
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AExitDoorTrue::OnBeginOverlap);
}

void AExitDoor_First::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent , OtherActor , OtherComp , OtherBodyIndex , bFromSweep , SweepResult);
	// 시간지연 주고 end event 발생시키기
	auto* Target = Cast<ATestCharacter>(OtherActor);
	if(Target)
	{
		EndEvent();
	}
}

void AExitDoor_First::RotateDoor()
{
	Super::RotateDoor();
}

void AExitDoor_First::EndEvent()
{
	UE_LOG(LogTemp,Warning,TEXT("AExitDoor_First::EndEvent"))
	FTimerHandle Timer2;
	GetWorldTimerManager().SetTimer(Timer2,[this]()
	{
		// 엔딩 bgm
		if (GameEndingwidget)
		{
			GameEndingUI=CreateWidget<UNormalGameEndingWidget>(GetWorld(),GameEndingwidget);
		}
		if (GameEndingUI)
		{
			GameEndingUI->AddToViewport(3);
			GameEndingUI->PlayEndingAnim();
		}
		// 엔딩 ui 
		if (EndEventSoundWave)
		{
			UGameplayStatics::PlaySound2D(this, EndEventSoundWave);
		}
	},3.f,false);

	FTimerHandle Timer3;
	GetWorldTimerManager().SetTimer(Timer3,[this]()
	{
		if (!StartLevel.IsValid())
		{
			StartLevel.LoadSynchronous(); 
		}
		
		UE_LOG(LogTemp,Warning,TEXT("AExitDoor_First:: Timer3"))
		if (StartLevel)
		{
			UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(),StartLevel,true);
			UE_LOG(LogTemp,Warning,TEXT("AExitDoor_First:: OpenLevel"))
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("AExitDoor_First:: StartLevel null"))
		}
	},17.5f,false);
}
