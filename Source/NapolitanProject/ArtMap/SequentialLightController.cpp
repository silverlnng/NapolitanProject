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
    FlickerDuration = 0.5f;
    FlickerIntensity = 0.5f;
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
void ASequentialLightController::StartLightOffSequence(float Interval)
{
    //UE_LOG(LogTemp, Warning, TEXT("StartLightOffSequence with interval: %f"), Interval);
    // 이미 진행 중이면 리턴
    if (bSequenceActive)
        return;
        
    // 조명이 없으면 리턴
    if (LightActors.Num() == 0)
        return;
    
    // 간격 설정
    IntervalBetweenLights = Interval;
    
    // 모든 조명 먼저 켜기
    ResetAllLights();
    
    // 시퀀스 활성화
    bSequenceActive = true;
    
    // 첫 번째 조명부터 시작
    CurrentLightIndex = 0;
    
    // 첫 번째 조명 끄기
    TurnOffNextLight();
}

// 조명 끄기 시퀀스 시작 (간격 조절 가능)
void ASequentialLightController::StartSequentialLightOffWithInterval(float Interval)
{
    // 이미 진행 중이면 리턴
    if (bSequenceActive)
        return;
        
    // 조명이 없으면 리턴
    if (LightActors.Num() == 0)
        return;
    
    // 간격 설정
    IntervalBetweenLights = Interval;
    
    // 모든 조명 먼저 켜기
    ResetAllLights();
    
    // 시퀀스 활성화
    bSequenceActive = true;
    
    // 첫 번째 조명부터 시작
    CurrentLightIndex = 0;
    
    // 첫 번째 조명 끄기 타이머 설정
    GetWorld()->GetTimerManager().SetTimer(
        LightOffTimerHandle,
        this,
        &ASequentialLightController::TurnOffNextLight,
        IntervalBetweenLights,
        false
    );
}

// 조명 켜기 시퀀스 시작 (갑자기 모두 켜기)
void ASequentialLightController::StartSuddenLightOnSequence(float DelayBeforeTurnOn)
{
    // 이미 모든 조명이 꺼져 있어야 함
    // 원하는 경우 먼저 모든 조명을 끌 수 있음
    TurnOffAllLightsImmediately();
    
    // 지정된 시간 후 모든 조명을 갑자기 켜기
    GetWorld()->GetTimerManager().SetTimer(
        SuddenLightOnTimerHandle,
        this,
        &ASequentialLightController::SuddenlyTurnOnAllLights,
        DelayBeforeTurnOn,
        false
    );
}

// 조명 하나씩 켜기 시퀀스 시작
void ASequentialLightController::StartSequentialLightOnSequence(float Interval)
{
    // 이미 진행 중이면 리턴
    if (bSequenceActive)
        return;
        
    // 조명이 없으면 리턴
    if (LightActors.Num() == 0)
        return;
    
    // 모든 조명 먼저 끄기
    TurnOffAllLightsImmediately();
    
    // 간격 설정
    IntervalBetweenLights = Interval;
    
    // 시퀀스 활성화
    bSequenceActive = true;
    
    // 첫 번째 조명부터 시작
    CurrentLightIndex = 0;
    
    // 첫 번째 조명 켜기
    TurnOnNextLight();
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

// 다음 조명 켜기
void ASequentialLightController::TurnOnNextLight()
{
    // 모든 조명 처리 완료 확인
    if (CurrentLightIndex >= LightActors.Num())
    {
        bSequenceActive = false;
        return;
    }
    
    // 현재 인덱스의 조명 켜기
    if (LightActors[CurrentLightIndex])
    {
        TurnOnLight(LightActors[CurrentLightIndex]);
    }
    
    // 다음 인덱스로 이동
    CurrentLightIndex++;
    
    // 다음 조명 켜기 예약 (타이머 사용)
    if (CurrentLightIndex < LightActors.Num())
    {
        GetWorld()->GetTimerManager().SetTimer(
            LightOnTimerHandle,
            this,
            &ASequentialLightController::TurnOnNextLight,
            IntervalBetweenLights,
            false
        );
    }
    else
    {
        // 모든 조명을 켠 후 시퀀스 종료
        bSequenceActive = false;
    }
}

// 조명 끄기 함수
void ASequentialLightController::TurnOffLight(AActor* LightActor)
{
    if (!LightActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("LightActor is null"));
        return;
    }
        
    // 범용적인 처리: 모든 조명 컴포넌트를 찾아서 처리
    TArray<ULightComponent*> LightComponents;
    LightActor->GetComponents<ULightComponent>(LightComponents);
    
    if (LightComponents.Num() > 0)
    {
        for (ULightComponent* Light : LightComponents)
        {
            if (Light)
            {
                Light->SetVisibility(false);
                UE_LOG(LogTemp, Warning, TEXT("Turned off light: %s"), *Light->GetName());
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No light components found in actor: %s"), *LightActor->GetName());
    }
}

// 조명 켜기 함수
void ASequentialLightController::TurnOnLight(AActor* LightActor)
{
    if (!LightActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("LightActor is null"));
        return;
    }
    
    // 범용적인 처리: 모든 조명 컴포넌트를 찾아서 처리
    TArray<ULightComponent*> LightComponents;
    LightActor->GetComponents<ULightComponent>(LightComponents);
    
    if (LightComponents.Num() > 0)
    {
        for (ULightComponent* Light : LightComponents)
        {
            if (Light)
            {
                Light->SetVisibility(true);
                UE_LOG(LogTemp, Warning, TEXT("Turned on light: %s"), *Light->GetName());
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No light components found in actor: %s"), *LightActor->GetName());
    }
}

// 모든 조명 다시 켜기
void ASequentialLightController::ResetAllLights()
{
    for (AActor* LightActor : LightActors)
    {
        if (!LightActor)
            continue;
            
        TurnOnLight(LightActor);
    }
    
    // 시퀀스 상태 초기화
    bSequenceActive = false;
    CurrentLightIndex = 0;
}

// 모든 조명 즉시 끄기
void ASequentialLightController::TurnOffAllLightsImmediately()
{
    for (AActor* LightActor : LightActors)
    {
        if (!LightActor)
            continue;
            
        TurnOffLight(LightActor);
    }
    
    // 시퀀스 상태 초기화
    bSequenceActive = false;
    CurrentLightIndex = 0;
}

// 모든 조명 즉시 켜기
void ASequentialLightController::TurnOnAllLightsImmediately()
{
    for (AActor* LightActor : LightActors)
    {
        if (!LightActor)
            continue;
            
        TurnOnLight(LightActor);
    }
}

// 갑자기 모든 조명 켜기 (효과 추가)
void ASequentialLightController::SuddenlyTurnOnAllLights()
{
    // 모든 조명 켜기
    TurnOnAllLightsImmediately();
    
    UE_LOG(LogTemp, Warning, TEXT("All lights turned on suddenly"));
}