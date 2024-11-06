// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllableLightActor.h"

#include "Components/RectLightComponent.h"
#include "Components/SphereComponent.h"
#include "NapolitanProject/NPC/NPC_Security.h"

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

	RectLightComp0=CreateDefaultSubobject<URectLightComponent>(TEXT("RectLightComponent"));
	RectLightComp0->SetupAttachment(RootComponent);



	
}

// Called when the game starts or when spawned
void AControllableLightActor::BeginPlay()
{
	Super::BeginPlay();
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
	// OtherActor->
	// OtherActor 가 플레이어이고 플레이어가 interact 버튼 누를때 라이트가 켜지도록 작동
	ANPC_Security* NPC_Security= Cast<ANPC_Security>(OtherActor);
	if (IsTurnOn&&NPC_Security)
	{
		TurnOnLight(false);
		NPC_Security->MinimumLightDist=100000;
	}
}


void AControllableLightActor::TurnOnLight(bool value)
{
	if (value)
	{
		// 라이트를 켜주기
		RectLightComp0->SetIntensity(RectLightIntensity);
		IsTurnOn=true;
	}
	else
	{
		// 라이트를 꺼주기 => 경비가 실행시키는 부분
		RectLightComp0->SetIntensity(0);
		IsTurnOn=false;
	}
}

