// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RandomStatueEvent.generated.h"

UENUM(BlueprintType)
enum class ERandomEventType : uint8
{
	HideMeshes      UMETA(DisplayName = "Hide Meshes"),
	RotateStatue    UMETA(DisplayName = "Rotate Statue")
};

UCLASS()
class NAPOLITANPROJECT_API ARandomStatueEvent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARandomStatueEvent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 트리거 이벤트 함수들
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							  bool bFromSweep, const FHitResult& SweepResult);

public:
	// 에디터에서 설정할 수 있는 프로퍼티들

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;
    
	// 사라질 메쉬들의 배열
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Random Statue Events|Mesh Events")
	TArray<AActor*> MeshesToHide;
    
	// 회전할 조각상들의 배열
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Random Statue Events|Rotation Events")
	TArray<AActor*> StatuesToRotate;
    
	// Z축 회전 각도 범위 (도)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Random Statue Events|Rotation Events", meta = (ClampMin = "0.0", ClampMax = "360.0"))
	float MinRotationAngle = 30.0f;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Random Statue Events|Rotation Events", meta = (ClampMin = "0.0", ClampMax = "360.0"))
	float MaxRotationAngle = 90.0f;
    
	// 이벤트 재사용 가능 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Random Statue Events|General")
	bool bCanRetrigger = true;
    
	// 이벤트 재사용 대기 시간 (초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Random Statue Events|General", meta = (ClampMin = "0.0"))
	float RetriggerDelay = 5.0f;


public:
	// 내부 로직 함수들
	void ExecuteRandomEvent();
	void ExecuteHideMeshesEvent();
	void ExecuteRotateStatueEvent();
	void ResetTrigger();
    
	// 타이머 핸들
	FTimerHandle RetriggerTimerHandle;
    
	// 상태 관리
	bool bIsTriggered = false;
	TArray<bool> OriginalMeshVisibility;
	TArray<FRotator> OriginalStatueRotations;

};
