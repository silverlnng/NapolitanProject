// Fill out your copyright notice in the Description page of Project Settings.


#include "LightHouse.h"

#include "Components/SpotLightComponent.h"

// Sets default values
ALightHouse::ALightHouse()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 루트 컴포넌트 생성
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// 등대 메시 컴포넌트 생성
	LightHouseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightHouseMesh"));
	LightHouseMesh->SetupAttachment(RootComponent);

	// 빛 회전용 루트 생성 (등대 상단에 위치)
	LightRotationRoot = CreateDefaultSubobject<USceneComponent>(TEXT("LightRotationRoot"));
	LightRotationRoot->SetupAttachment(LightHouseMesh);
	// 등대 상단으로 위치 조정 (Z축으로 올림)
	LightRotationRoot->SetRelativeLocation(FVector(0.0f, 0.0f, 800.0f));

	// 스포트라이트 생성
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(LightRotationRoot);
	// 스포트라이트를 앞쪽으로 향하도록 설정
	SpotLight->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	SpotLight->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

}

// Called when the game starts or when spawned
void ALightHouse::BeginPlay()
{
	Super::BeginPlay();

	// 기본 설정값
	RotationSpeed = 30.0f;          // 30도/초
	//LightIntensity = 10000.0f;      // 강한 빛
	//LightColor = FLinearColor::White;
	SpotLightAngle = 45.0f;         // 45도 각도
	//LightDistance = 5000.0f;        // 5000 유닛 도달거리
	bEnableBlinking = false;        // 깜빡임 비활성화
	BlinkingInterval = 2.0f;        // 2초 주기

	// 내부 변수 초기화
	CurrentRotation = 0.0f;
	BlinkingTimer = 0.0f;
	bIsLightOn = true;

	// 스포트라이트 초기 설정
	if (SpotLight)
	{
		//SpotLight->SetIntensity(LightIntensity);
		//SpotLight->SetLightColor(LightColor);
		SpotLight->SetInnerConeAngle(SpotLightAngle * 0.5f);
		SpotLight->SetOuterConeAngle(SpotLightAngle);
		//SpotLight->SetAttenuationRadius(LightDistance);
		SpotLight->SetVisibility(bIsLightOn);
        
		// 그림자 설정
		SpotLight->SetCastShadows(true);
		//SpotLight->SetShadowResolution(2048);
        
		// 빛의 품질 설정
		SpotLight->SetLightFalloffExponent(2.0f);
		SpotLight->SetSourceRadius(50.0f);
		SpotLight->SetSoftSourceRadius(100.0f);
	}
	
}

// Called every frame
void ALightHouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 빛 회전 처리
	RotateLight(DeltaTime);

	// 깜빡임 처리
	if (bEnableBlinking)
	{
		HandleBlinking(DeltaTime);
	}
}

void ALightHouse::RotateLight(float DeltaTime)
{
	if (LightRotationRoot)
	{
		// 회전 각도 업데이트
		CurrentRotation += RotationSpeed * DeltaTime;
        
		// 360도를 넘으면 초기화
		if (CurrentRotation >= 360.0f)
		{
			CurrentRotation -= 360.0f;
		}

		// 회전 적용 (Z축 기준으로 회전)
		FRotator NewRotation = FRotator(0.0f, CurrentRotation, 0.0f);
		LightRotationRoot->SetRelativeRotation(NewRotation);
	}
}

void ALightHouse::HandleBlinking(float DeltaTime)
{
	if (SpotLight)
	{
		BlinkingTimer += DeltaTime;
        
		// 깜빡임 주기에 도달하면 상태 변경
		if (BlinkingTimer >= BlinkingInterval)
		{
			bIsLightOn = !bIsLightOn;
			SpotLight->SetVisibility(bIsLightOn);
			BlinkingTimer = 0.0f;
		}
	}
}

