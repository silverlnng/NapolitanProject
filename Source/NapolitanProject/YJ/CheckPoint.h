// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NapolitanProject/Interface/InteractInterface.h"
#include "CheckPoint.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ACheckPoint : public AActor,public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class ATestCharacter* MainCharacter;
	class ATestPlayerController* TestPC;
	class APlayerHUD* PlayerHUD;
	class UMyTestGameInstance* GI;
	class USaveGISubsystem* SaveGI;
	
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp;
	
	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly)
	class UCapsuleComponent* CapsuleComp;
	
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SaveLocComp;
	
	// 캐릭터가 상호작용 눌렀을때 실행시킬 함수
	virtual void IInteract() override;
	UFUNCTION()
	void VisibleSaveWidget();
	
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditDefaultsOnly) // 블프에서 정의하기 
	FString SaveLocation=FString(TEXT("2층 전시관 앞"));

	// 로드해야하는 서브레벨들을 블프에서 할당하기 
	UPROPERTY(EditDefaultsOnly)
	TArray<TSoftObjectPtr<UWorld>> SubLevelArray;
	
	// 현재 인덱스
	int32 CurrentIndex = 0;

	// 타이머 핸들
	FTimerHandle loadSubLevelTimerHandle;

	// 반복 실행 함수
	void ProcessNextSubLevel();
};

