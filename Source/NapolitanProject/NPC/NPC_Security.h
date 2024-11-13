// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "NPC_Security.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ESecurityState:uint8
{
	ChasePlayer,
	Patrol,
	TurnOff,
	Attack,
	Stop
};


UCLASS()
class NAPOLITANPROJECT_API ANPC_Security : public ANPCCharacter
{
	GENERATED_BODY()
	ANPC_Security();
public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual int32 GetNPCID() override;

	int32 NPC_ID =4;
	
	UPROPERTY(VisibleAnywhere)
	ESecurityState SecurityState= ESecurityState::Patrol;
	
	void SetState(ESecurityState curState);

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	UAudioComponent* FootstepAudioComponent;

	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundCue* FootstepSoundCue;

	UPROPERTY(EditAnywhere, Category = "Sound")
	float MaxFootstepVolume = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Sound")
	float MaxDistance = 1000.0f;
	// 돌아다닐때 거리에따라 소리

	// 라이트가 꺼져있다 : 랜덤한 위치으로 돌아다니기
	// 라이트가 꺼지고 랜덤으로 돌아다니 던 중 , 캐릭터 마주치면 캐릭터를 chase

	// 월드에있는 라이트가 켜져있는지 꺼져있는지 여부를 어떻게 확인 ??
	// 라이트가 켜져있으면 라이트를 향해 이동
	// 켜져있는 상태에서 캐릭터 마주치면 캐릭터를 우선순위로 삼아 chase
		// 캐릭터가 일정범위안.. 또는 시야각 안에있을때 우선순위를 가지게 됨

	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSensingComp;
	
	UPROPERTY(VisibleAnywhere)
	TArray<class AControllableLightActor*> ControllableLightArray;
	UPROPERTY(VisibleAnywhere)
	float MinimumLightDist =100000;
	UPROPERTY(VisibleAnywhere)
	AControllableLightActor* NearLight;
	UPROPERTY(VisibleAnywhere)
	bool AllLightTurnOff;
	UPROPERTY(VisibleAnywhere)
	APawn* Target;
	UPROPERTY(VisibleAnywhere)
	ATestCharacter* MainCha;

	UPROPERTY(EditAnywhere,Category=Speed)
	float PatrolSpeed;
	UPROPERTY(EditAnywhere,Category=Speed)
	float ChaseSpeed;
	
	UFUNCTION()
	void OnSeePawn(APawn *OtherPawn);


	
	void TickChasePlayer(const float& DeltaTime);
	void TickPatrol(const float& DeltaTime);
	void TickTurnOff(const float& DeltaTime);
	void TickAttack(const float& DeltaTime);
	void TickAllStop(const float& DeltaTime);

	// 네비게이션을 이용해서 길찾기를 하고싶다.
	UPROPERTY(VisibleAnywhere)
	class AAIController* EnemyAI;

	FVector PatrolPoint;
	float PatrolPointRadius = 700;

	// 공격 가능거리
	float AttackDistance = 300;

	// 내위치에서 반경 5미터
	bool SetPatrolPoint(FVector origin, float radius, FVector& dest);

	UPROPERTY()
	class UNPC_Security_AnimInstance* Anim;

	// 애니메이션에서 발생한 알림을 받고싶다.
	void OnMyAttackStart();
	void OnMyAttackEnd();

////////// 카메라를 skeletalMesh의 자식으로 
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArmComp2;
	
	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* CameraComp2;

	virtual void ChangeCleared() override;
	void EndEvent();
	

	UPROPERTY(EditDefaultsOnly,Category=Item)
	class UStaticMeshComponent* HeadStaticMesh;
	

	UPROPERTY(EditDefaultsOnly,Category=Item)
	TSubclassOf<class AItemActor> ItemHeadBP;
	
	UPROPERTY(EditDefaultsOnly,Category=Item)
	class AItemActor* ItemHead;
};

