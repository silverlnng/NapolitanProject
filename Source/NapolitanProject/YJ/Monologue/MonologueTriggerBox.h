// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonologueTriggerBox.generated.h"

UCLASS()
class NAPOLITANPROJECT_API AMonologueTriggerBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonologueTriggerBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	int32 Monologue_ID =-1;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp;
	
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;

	class ATestCharacter* MainCharacter;
	class ATestPlayerController* TestPC;
	class APlayerHUD* PlayerHUD;
	
	UFUNCTION()
	void BoxCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void BoxCompEndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 출력할 독백을 배열로 정의해두기
	UPROPERTY(EditAnywhere)
	TArray<FString> TextLines;
	
	// 현재 출력 중인 인덱스
	int32 CurrentIndex;

	// 이벤트가 한 번만 실행되도록 제어하는 플래그
	bool bHasTriggered;
	
	// 타이머 핸들러
	FTimerHandle TimerHandle;

};
