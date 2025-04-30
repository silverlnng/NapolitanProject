// SequentialLightController.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SpotLightComponent.h"
#include "Components/PointLightComponent.h"
#include "SequentialLightController.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ASequentialLightController : public AActor
{
	GENERATED_BODY()
    
public:    
	// 기본 세팅
	ASequentialLightController();
    
	// 매 프레임 호출
	virtual void Tick(float DeltaTime) override;

public:
	// 게임 시작 시 호출
	virtual void BeginPlay() override;
    
	// 조명 목록 (에디터에서 설정 가능)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Control")
	TArray<AActor*> LightActors;
    
	// 타이머 관련 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Control")
	float IntervalBetweenLights;
    
	// 현재 끄고 있는 조명 인덱스
	UPROPERTY(BlueprintReadOnly, Category = "Light Control")
	int32 CurrentLightIndex;
    
	// 조명 끄기 진행 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Control")
	bool bSequenceActive;
    
	// 타이머 핸들
	FTimerHandle LightOffTimerHandle;
    
	// 조명 순차적으로 끄기 시작
	UFUNCTION(BlueprintCallable, Category = "Light Control")
	void StartLightOffSequence();
    
	// 다음 조명 끄기
	UFUNCTION()
	void TurnOffNextLight();
    
	// 실제 조명 끄기 실행
	UFUNCTION(BlueprintCallable, Category = "Light Control")
	void TurnOffLight(AActor* LightActor);
    
	// 모든 조명 다시 켜기
	UFUNCTION(BlueprintCallable, Category = "Light Control")
	void ResetAllLights();
};