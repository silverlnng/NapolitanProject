// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NapolitanProject/Interact/SouvenirActor.h"
#include "NapolitanProject/NPC/NPCCharacter.h"
#include "NPC_LeeSeo.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ANPC_LeeSeo : public ANPCCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC_LeeSeo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	virtual void ResultEvent(int32 result) override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 NPC_ID = 7; //최이서의 번호

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 State = 1;
	
	virtual void Interact() override;

	virtual int32 GetNPCID() override;

	virtual int32 GetState() override;

	virtual void ChangeCleared() override; //사라지고 난 뒤 클리어

public:
	//0-1. 비명 소리
	UPROPERTY(EditAnywhere)
	class USoundBase* ScreamSound;

	//맵에서 나오는 소름돋는 사운드
	UPROPERTY(EditAnywhere)
	class USoundBase* LSSound;

	//이서 점프스케어 시 낄낄 웃는 사운드
	UPROPERTY(EditAnywhere)
	class USoundBase* LSJumpSkareSound;

	//칼에 찔리는 소리
	UPROPERTY(EditAnywhere)
	class USoundBase* KnifeStabbingSound;

	UPROPERTY(EditAnywhere, Category = "Spawn Items")
	TSubclassOf<ASouvenirActor> SouvenirClass;

	//유품 스폰 위치
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Point")
	TSubclassOf<AActor> LeeseoPoint;
	
	//점프스퀘어 발동 변수
	bool bJumpSquare = false;

	
public:
	//유품을 스폰하는 함수
	void SpawnItem();

	//BoxCollision 제거
	void RemoveBPBoxCollision();

	//노래 재생
	void KnifeSound();

	//사망 이벤트 생성
public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> DeathCameraShakeClass; //카메라 쉐이크

	UPROPERTY(EditDefaultsOnly)
	class UNPC_LeeSeoAnimInstance* Anim;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundWave* AttackSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* MonsterCamera; //점프스케어 카메라 - 사망 시 사용 

	UPROPERTY()
	class ASequentialLightController* LightControlReference;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* KnifeHandle;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* Knifeblade;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* Knife;

	//사망용 스크립트
	UPROPERTY(EditAnywhere)
	FString description=FString(TEXT("금발 소녀에게 사망했다"));

	//이동 관련 변수
	bool bShouldMove = false; //움직일지 말지 정하는것
	float MoveTimeRemaining = 0.0f; //달리는 시간 감소
	float MoveSpeed = 1.0f; // 이동 입력 강도 (1.0이 기본)

	//이동 시작 함수
	void StartMovingForward(float Duration, float Speed);

	//점프스케어 함수들
public:

	//플레이어의 카메라를 몬스터 카메라로 전환하는 함수 
	void SwitchToMonsterCamera();
	
	//모든 타이머를 순차적으로 실행하는 함수 구현
	void ExecuteJumpScareSequence();

	//캐릭터 숨기는 함수
	void HideMesh();

	//달려오는 애니메이션
	void RunAnim();

	//카메라 전환 및 공격 애니메이션
	void AttackScare();

	//사망 UI 표시 함수
	void CreateDieUI();

	void SetActorViewTarget(UCameraComponent* TargetCamera);

	void HideKnife(bool IsSee);
	
	
	
};
