// SequentialLightController.cpp
#include "SequentialLightController.h"
#include "Engine/World.h"
#include "TimerManager.h"

// 생성자
ASequentialLightController::ASequentialLightController()
{
    // Tick 함수 활성화
    PrimaryActorTick.bCanEverTick = true;
    
    // 기본값 설정
    IntervalBetweenLights = 1.0f;
    CurrentLightIndex = 0;
    bSequenceActive = false;
}

// 게임 시작 시 호출
void ASequentialLightController::BeginPlay()
{
    Super::BeginPlay();
}

// 매 프레임 호출
void ASequentialLightController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// 조명 끄기 시퀀스 시작
void ASequentialLightController::StartLightOffSequence()
{
    // 이미 진행 중이면 리턴
    if (bSequenceActive)
        return;
        
    // 조명이 없으면 리턴
    if (LightActors.Num() == 0)
        return;
    
    // 모든 조명 먼저 켜기
    ResetAllLights();

    UE_LOG(LogTemp, Warning, TEXT("Light1"));
    
    // 시퀀스 활성화
    bSequenceActive = true;
    
    // 첫 번째 조명부터 시작
    CurrentLightIndex = 0;
    
    // 첫 번째 조명 끄기
    TurnOffNextLight();
    UE_LOG(LogTemp, Warning, TEXT("Light2"));
}

// 다음 조명 끄기
void ASequentialLightController::TurnOffNextLight()
{
    // 모든 조명 처리 완료 확인
    if (CurrentLightIndex >= LightActors.Num())
    {
        bSequenceActive = false;
        return;
    }
    
    // 현재 인덱스의 조명 끄기
    if (LightActors[CurrentLightIndex])
    {
        TurnOffLight(LightActors[CurrentLightIndex]);
    }
    
    // 다음 인덱스로 이동
    CurrentLightIndex++;
    
    // 다음 조명 끄기 예약 (타이머 사용)
    if (CurrentLightIndex < LightActors.Num())
    {
        GetWorld()->GetTimerManager().SetTimer(
            LightOffTimerHandle,
            this,
            &ASequentialLightController::TurnOffNextLight,
            IntervalBetweenLights,
            false
        );
    }
    else
    {
        // 모든 조명을 끈 후 시퀀스 종료
        bSequenceActive = false;
    }
}

// 조명 끄기 함수
void ASequentialLightController::TurnOffLight(AActor* LightActor)
{
    if (!LightActor)
        return;
        
    // SpotLight 컴포넌트 찾기
    USpotLightComponent* SpotLight = LightActor->FindComponentByClass<USpotLightComponent>();
    if (SpotLight)
    {
        SpotLight->SetVisibility(false);
        return;
    }
    
    // PointLight 컴포넌트 찾기
    UPointLightComponent* PointLight = LightActor->FindComponentByClass<UPointLightComponent>();
    if (PointLight)
    {
        PointLight->SetVisibility(false);
        return;
    }
    
    // 다른 조명 컴포넌트도 필요하다면 여기에 추가
}

// 모든 조명 다시 켜기
void ASequentialLightController::ResetAllLights()
{
    for (AActor* LightActor : LightActors)
    {
        if (!LightActor)
            continue;
            
        // SpotLight 컴포넌트 찾기
        USpotLightComponent* SpotLight = LightActor->FindComponentByClass<USpotLightComponent>();
        if (SpotLight)
        {
            SpotLight->SetVisibility(true);
            continue;
        }
        
        // PointLight 컴포넌트 찾기
        UPointLightComponent* PointLight = LightActor->FindComponentByClass<UPointLightComponent>();
        if (PointLight)
        {
            PointLight->SetVisibility(true);
            continue;
        }
    }
    
    // 시퀀스 상태 초기화
    bSequenceActive = false;
    CurrentLightIndex = 0;
}