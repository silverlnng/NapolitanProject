// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EventTriggerBox.generated.h"

UENUM(BlueprintType)
enum class ETriggerSide : uint8
{
	None    UMETA(DisplayName = "None"),
	Left    UMETA(DisplayName = "Left"),
	Right   UMETA(DisplayName = "Right")
};

UCLASS()
class NAPOLITANPROJECT_API AEventTriggerBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEventTriggerBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp;
	
	UPROPERTY(VisibleAnywhere)
	class UBillboardComponent* EditorBillboard;  // 시각화용 컴포넌트

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* TriggerLeft;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* TriggerRight;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* MessageWidget;
	
//////////////////////////////////////////////////////////////////////////////////////////////	
	class ATestCharacter* MainCharacter;
	class ATestPlayerController* TestPC;
	class APlayerHUD* PlayerHUD;
	class UMyTestGameInstance* GI;

	UFUNCTION()
	virtual void BindBeginOverlap();
	
	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void BindEndOverlap();
	
	UFUNCTION()
	virtual void EndOverlap( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

//////////////////////////////////////////////////////////////////////////////////

	void WhenLToR();
	
	void WhenRToL();

	void UnLoadLevel(TArray<TSoftObjectPtr<UWorld>>* UnLoadLevelArray);
	
	// 로드해야하는 서브레벨들을 블프에서 할당하기 
	UPROPERTY(EditDefaultsOnly,Category="LevelArray")
	TArray<TSoftObjectPtr<UWorld>> LToR_LoadLevelArray;

	FTimerHandle LoadSubLevelTimerHandle;
	int32 LoadUUIDCounter = 0;
	

	
	// 언로드 해야되는 서브레벨들 
	UPROPERTY(EditDefaultsOnly,Category="LevelArray")
	TArray<TSoftObjectPtr<UWorld>> LToR_UnLoadLevelArray;

	FTimerHandle UnLoadSubLevelTimerHandle;
	int32 UnLoadUUIDCounter = 0;


	// 로드해야하는 서브레벨들을 블프에서 할당하기 
	UPROPERTY(EditDefaultsOnly,Category="LevelArray")
	TArray<TSoftObjectPtr<UWorld>> RToL_LoadLevelArray;

	
	// 언로드 해야되는 서브레벨들 
	UPROPERTY(EditDefaultsOnly,Category="LevelArray")
	TArray<TSoftObjectPtr<UWorld>> RToL_UnLoadLevelArray;
	
//////////////////////////////방향 판정 //////////////////////////////////////

	
	TMap<AActor*, ETriggerSide> FirstTriggerMap;
	
	void HandleDirection(AActor* PlayerActor, ETriggerSide FirstSide, ETriggerSide SecondSide);
	
	// 현재 인덱스
	int32 CurrentIndex = 0;
	
	// 반복 실행 함수
	virtual void ProcessNextSubLevel();
};
