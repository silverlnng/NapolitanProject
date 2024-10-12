// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPC_Youngsoo.generated.h"

//  ü     
UENUM(BlueprintType)
enum class NYSState : uint8 {
	YSFirst,
	YSSecond,
	YSThird,
	YSRandom,
	YSMove,
	YSChase,
};

UCLASS()
class NAPOLITANPROJECT_API ANPC_Youngsoo : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC_Youngsoo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	//상태 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	NYSState mState = NYSState::YSRandom; //일단 처음엔 상태를 랜덤 지정

	//상태 함수
	UFUNCTION(BlueprintCallable, Category = State)
	void YSFirstState(); //미술 작품을 보면서 울고 있는 경우

	UFUNCTION(BlueprintCallable, Category = State)
	void YSSecondState(); //스카프를 불태워야하는 경우

	UFUNCTION(BlueprintCallable, Category = State)
	void YSThirdState(); //남자가 작품을 보며 웃을 때 -> 그 영역 안에 들어왓을 때 추적 후 무조건 사망

	UFUNCTION(BlueprintCallable, Category = State)
	NYSState YSRandomState(); //랜덤으로 상태지정

	UFUNCTION(BlueprintCallable, Category = State)
	void YSMoveState(); //조건 해금 후 움직인 다음 플레이어의 시야에서 사라짐

	UFUNCTION(BlueprintCallable, Category = State)
	void YSChaseState(); //미술 작품을 보면서 울고 있는 경우

	UFUNCTION()
	void SetState(NYSState newState);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCapsuleComponent* YSCol; // 만약 닿을 시에 선택지 UI 생성

	bool bIsChat = false;

	UFUNCTION()
	void ChoiceUIOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);


	UFUNCTION()
	void initNYSUI();

	UPROPERTY()
	class ATestCharacter* player; //플레이어

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class APlayerHUD* huds; //HUD

};
