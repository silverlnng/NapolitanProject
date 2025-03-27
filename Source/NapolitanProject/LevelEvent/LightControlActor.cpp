// Fill out your copyright notice in the Description page of Project Settings.


#include "LightControlActor.h"
#include "TimerManager.h"
#include "Components/LightComponent.h"
#include "Engine/Light.h"

// Sets default values
ALightControlActor::ALightControlActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALightControlActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALightControlActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALightControlActor::StartSineFlicker(int32 StartIndex, int32 Count,int32 FlickerDuration)
{
	if (FlickerTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(FlickerTimerHandle);
	}

	// 범위에 있는 라이트만 따로 배열에 저장
	SelectedLights.Empty();
	SelectedLightsOriginIntensity.Empty();
	
	for (int32 i = StartIndex; i < StartIndex + Count; i++)
	{
		if (ManagedLights.IsValidIndex(i))
		{
			SelectedLights.Add(ManagedLights[i]);
			float Originintensity=ManagedLights[i]->GetLightComponent()->Intensity;
			SelectedLightsOriginIntensity.Add(Originintensity);
		}
	}

	// 선택된 라이트에만 Flicker 타이머 시작
	GetWorldTimerManager().SetTimer(FlickerTimerHandle, this, &ALightControlActor::UpdateFlicker, FlickerUpdateRate, true);

	GetWorldTimerManager().SetTimer(AutoStopHandle, this, &ALightControlActor::StopSineFlicker, FlickerDuration, false);
}

void ALightControlActor::StopSineFlicker()
{
	if (FlickerTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(FlickerTimerHandle);

		// 멈출 때 라이트를 원래 밝기로 복구하거나 끄기
		for (int32 i=0; i< SelectedLights.Num(); i++)
		{
			ALight* Light =SelectedLights[i];
			BaseIntensity=SelectedLightsOriginIntensity[i];
			if (Light && Light->GetLightComponent()&&BaseIntensity)
			{
				Light->GetLightComponent()->SetIntensity(BaseIntensity);
			}
		}
	}
}

void ALightControlActor::UpdateFlicker()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	for (int32 i=0; i< SelectedLights.Num(); i++)
	{
		ALight* Light =SelectedLights[i];
		if (Light && Light->GetLightComponent())
		{
			BaseIntensity=SelectedLightsOriginIntensity[i];
			
			//float Offset = static_cast<float>(Light->GetUniqueID() % 1000) * 0.1f;
			
			float NewIntensity = BaseIntensity 
				+ FMath::Sin(CurrentTime * FlickerSpeed) * Amplitude;
			
			Light->GetLightComponent()->SetIntensity(NewIntensity);
		}
	}
}

