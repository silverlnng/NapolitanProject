// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitDoor_First.h"

#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

void AExitDoor_First::BindBeginOverlap()
{
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AExitDoorTrue::OnBeginOverlap);
}

void AExitDoor_First::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent , OtherActor , OtherComp , OtherBodyIndex , bFromSweep , SweepResult);
}

void AExitDoor_First::RotateDoor()
{
	Super::RotateDoor();
	// 시간지연 주고 end event 발생시키기
	EndEvent();
}

void AExitDoor_First::EndEvent()
{
	FTimerHandle Timer2;
	GetWorldTimerManager().SetTimer(Timer2,[this]()
	{
		// 엔딩 bgm
		if (GameEndingwidget)
		{
			GameEndingUI=CreateWidget<UUserWidget>(GetWorld(),GameEndingwidget);
		}
		if (GameEndingUI)
		{
			GameEndingUI->AddToViewport(3);
		}
		// 엔딩 ui 
		if (EndEventSoundWave)
		{
			UGameplayStatics::PlaySound2D(this, EndEventSoundWave);
		}
	},2.f,false);
}
