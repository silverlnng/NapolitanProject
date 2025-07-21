// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NapolitanProject/NPC/NPCCharacter.h"
#include "DocentV2.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ADocentV2 : public ANPCCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADocentV2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	class ATestCharacter* MainCharacter;
	class ATestPlayerController* TestPC;
	class APlayerHUD* PlayerHUD;
	class AAIController* AIController;

	class ASoundControlActor* SoundControlActor;
	
	UPROPERTY(EditDefaultsOnly)
	class UAudioComponent* AudioComp;
	
	/** 몬스터 전용 카메라 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* MonsterCamera;

	UPROPERTY(EditDefaultsOnly)
	class USpotLightComponent* SpotLightComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 NPC_ID =2;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 State =1;

	virtual void Interact() override;

	virtual int32 GetNPCID() override;

	virtual int32 GetState() override;
	
	bool bOnlyOnce=false;
	
	/** 플레이어의 카메라를 몬스터 카메라로 전환하는 함수 */
	void SwitchToMonsterCamera();

	void StopAllTurnDetect();
	
	//3초 뒤돌아있고 ,
	void StartTurnDetect();

	void PlayTurnRightAnimation();

	void PlayTurnOriginAnimation();

///////////////// 플레이어 탐지부분 ////////////////////

	FTimerHandle StartDetectionTimerHandle;
	FTimerHandle StopDetectionTimerHandle;
	FTimerHandle PlayTurnRightAnim;
	FVector LastPlayerLocation;

	UPROPERTY(EditAnywhere)
	float DetectRange=1.0f;
	
	UPROPERTY(EditAnywhere)
	float MaxDetectionDistance = 1800.0f; // 감지 거리 제한 (단위: cm)
	
	bool bCanDetectMovement=false;
	bool InMaxDetectionDistance=false;
	void StartMovementDetection();
	
	void DetectPlayerMovement();

	void StopMovementDetection();
	
//////////////////////////////////////////////////////////////////
	bool bHasDetectedPlayer = false;
	
	UPROPERTY(EditAnywhere)
	float AttackRange = 300.0f;

	void PlayAttackAnimation();

	// 노트를 획득해서 사라지는거 만들어야 할때 
	void PickUPNote();

	// 노트획득, 가까이오는거 만들기
	void CloseUPCam();

	void DocentLightOn();

	void DetachDestroy();

	UPROPERTY(EditAnywhere)
	float LightIntensity=20.f;

	UPROPERTY(EditAnywhere) // 맵에서 할당하기 
	class ASouvenir_Docent* Souvenir_Docent=nullptr;

	UPROPERTY(EditAnywhere)
	FString description=FString(TEXT("도슨트에게 잡혔다"));
	
//////////////////////////////////////////////////////////////////////
	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* TurnAroundMontage;

	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* TurnOriginMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* RunMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* DetectMontage;

	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* GoingUpMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundWave* AttackSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundWave* DetectSound;
};
