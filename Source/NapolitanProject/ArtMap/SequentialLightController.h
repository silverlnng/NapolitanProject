// SequentialLightController.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SpotLightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/RectLightComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/LightComponent.h"
#include "SequentialLightController.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ASequentialLightController : public AActor
{
    GENERATED_BODY()
    
public:    
    // 생성자
    ASequentialLightController();

protected:
    // 게임 시작 시 호출
    virtual void BeginPlay() override;

public:    
    // 매 프레임 호출
    virtual void Tick(float DeltaTime) override;

    // 조명 끄기 시퀀스 시작 
    UFUNCTION(BlueprintCallable, Category = "Light Control")
    void StartLightOffSequence(float Interval = 1.0f);
    
    // 조명 켜기 시퀀스 시작 (갑자기 모두 켜기)
    UFUNCTION(BlueprintCallable, Category = "Light Control")
    void StartSuddenLightOnSequence(float DelayBeforeTurnOn = 1.0f);
    
    // 모든 조명 즉시 끄기
    UFUNCTION(BlueprintCallable, Category = "Light Control")
    void TurnOffAllLightsImmediately();
    
    // 모든 조명 즉시 켜기
    UFUNCTION(BlueprintCallable, Category = "Light Control")
    void TurnOnAllLightsImmediately();

protected:
    // 다음 조명 끄기
    void TurnOffNextLight();
    
    // 조명 끄기 함수
    void TurnOffLight(AActor* LightActor);
    
    // 조명 켜기 함수
    void TurnOnLight(AActor* LightActor);
    
    // 모든 조명 다시 켜기
    void ResetAllLights();
    
    // 갑자기 모든 조명 켜기
    void SuddenlyTurnOnAllLights();

public:
    // 관리할 조명 배열
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Control")
    TArray<AActor*> LightActors;
    
    // 조명 간 간격 (초)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Control")
    float IntervalBetweenLights;
    
    // 깜빡임 효과 지속 시간
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Control")
    float FlickerDuration;
    
    // 깜빡임 효과 강도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Control")
    float FlickerIntensity;

private:
    // 현재 처리 중인 조명 인덱스
    int32 CurrentLightIndex;
    
    // 시퀀스 활성화 상태
    bool bSequenceActive;
    
    // 타이머 핸들
    FTimerHandle LightOffTimerHandle;
    FTimerHandle SuddenLightOnTimerHandle;
    
    // 깜빡임 효과를 위한 변수들
    FTimerHandle FlickerTimerHandle;
    float OriginalIntensity;

    // SequentialLightController.h에 추가할 내용
public:
    // 조명 순차적으로 끄기 (간격 조절 가능)
    UFUNCTION(BlueprintCallable, Category = "Light Control")
    void StartSequentialLightOffWithInterval(float Interval);
    
    // 조명 하나씩 켜기 시퀀스 시작
    UFUNCTION(BlueprintCallable, Category = "Light Control")
    void StartSequentialLightOnSequence(float Interval);
    
    // 다음 조명 켜기
    void TurnOnNextLight();
    
private:
    // 타이머 핸들 추가
    FTimerHandle LightOnTimerHandle;
};