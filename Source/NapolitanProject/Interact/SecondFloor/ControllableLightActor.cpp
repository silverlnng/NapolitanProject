// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllableLightActor.h"

#include "AIController.h"
#include "Components/RectLightComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/NPC/Security/NPC_Security.h"

// Sets default values
AControllableLightActor::AControllableLightActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);

	SphereComp=CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComp->SetupAttachment(RootComponent);

	BtnMeshComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	BtnMeshComp->SetupAttachment(SphereComp);

	RectLightComp0=CreateDefaultSubobject<URectLightComponent>(TEXT("RectLightComponent0"));
	RectLightComp0->SetupAttachment(RootComponent);

	RectLightComp1=CreateDefaultSubobject<URectLightComponent>(TEXT("RectLightComponent1"));
	RectLightComp1->SetupAttachment(RootComponent);

	RectLightComp2=CreateDefaultSubobject<URectLightComponent>(TEXT("RectLightComponent2"));
	RectLightComp2->SetupAttachment(RootComponent);

	RectLightComp3=CreateDefaultSubobject<URectLightComponent>(TEXT("RectLightComponent3"));
	RectLightComp3->SetupAttachment(RootComponent);

	RectLightComp4=CreateDefaultSubobject<URectLightComponent>(TEXT("RectLightComponent4"));
	RectLightComp4->SetupAttachment(RootComponent);

	RectLightComp5=CreateDefaultSubobject<URectLightComponent>(TEXT("RectLightComponent5"));
	RectLightComp5->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AControllableLightActor::BeginPlay()
{
	Super::BeginPlay();

	//RectLightIntensity = 10.f;

	GetComponents<URectLightComponent>(RectLightArray);
	
	SphereComp->OnComponentBeginOverlap.AddDynamic(this,&AControllableLightActor::BtnMeshOverlap);
}

// Called every frame
void AControllableLightActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AControllableLightActor::BtnMeshOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// OtherActor 가 플레이어이고 플레이어가 interact 버튼 누를때 라이트가 켜지도록 작동
	NPC_Security= Cast<ANPC_Security>(OtherActor);
	if (IsTurnOn&&NPC_Security)
	{
		NPC_Security->EnemyAI->StopMovement();
		
		FTimerHandle DelayTimer;
		// 여기서 잠시 NPC_Security 를 멈추게 하고싶다.
		GetWorld()->GetTimerManager().SetTimer(DelayTimer,[this]()
		{
			TurnOnLight(false);
			NPC_Security->MinimumLightDist=100000;
		},1.5f,false);
	}
}


void AControllableLightActor::TurnOnLight(bool value)
{
	if (value)
	{
		// 라이트를 켜주기
		for (URectLightComponent* RectLight :RectLightArray)
		{
			RectLight->SetIntensity(RectLightIntensity);
		}
		IsTurnOn=true;
		// 소리
		if (LightSwitchSoundWave)
		{
			UGameplayStatics::PlaySound2D(this, LightSwitchSoundWave);
		}
	}
	else
	{
		// 라이트를 꺼주기 => 경비가 실행시키는 부분
		for (URectLightComponent* RectLight :RectLightArray)
		{
			RectLight->SetIntensity(offRectLightIntensity);
		}
		IsTurnOn=false;
	}
}

