// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NapolitanProject/NapolitanProjectCharacter.h"
#include "TestCharacter.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPlayerState:uint8
{
	Idle,
	Talking,
	Die,
	UI
};
// 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpecialInteraction, AActor*, InteractedActor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnablePlayerNoise);

UCLASS()
class NAPOLITANPROJECT_API ATestCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	//USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UFUNCTION()
	void MyJump();
	UFUNCTION()
	void MyJumpCompeleted();
	
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
public:
	ATestCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere)
	float LookSpeed=1;;
	
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

public:
	/** Returns Mesh1P subobject **/
	//USkeletalMeshComponent* GetMesh1P() const { return Mesh1P;
	
	UPROPERTY(VisibleAnywhere)
	class APlayerController* PC;

	UPROPERTY(VisibleAnywhere)
	class APlayerHUD* PlayerHUD;
	
	UPROPERTY(VisibleAnywhere)
	EPlayerState curState=EPlayerState::Idle;

	UFUNCTION(BlueprintCallable)
	void SetPlayerState(EPlayerState newState);

	UPROPERTY(VisibleAnywhere)
	bool curLevelISLobby=false;
	
	/////////////////// 달리기 기능 ////////////////////////
	float DefaultWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Run;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=run)
	float runSpeed = DefaultWalkSpeed*2.f;
	// 특정 키를 누르면 달리기
	// 누르고 있는 동안 && 쿨타임 내에서 
	float runCooltime =3.f;
	bool bIsRunning;
	bool bIsRunGageRemains;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=run)
	float RunGage =4.0f ;
	
	void OnRunAction(const FInputActionValue& Value);

	void EndRunAction(const FInputActionValue& Value);

	void UpdateRunAction(float DeltaTime);
	void UpdateNotRunAction(float DeltaTime);
	
	////////////////// ESC 키 기능//////////////////////////
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_ESC;

	UFUNCTION()
	void ESCUIToggle(const FInputActionValue& Value);

	// 테스트 용도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_P;
	
	///////////////////////////  인벤토리 ui /////////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Inventory;

	UFUNCTION()
	void InventoryUIToggle(const FInputActionValue& Value);

	UPROPERTY(BlueprintReadWrite, Category="Game Rules")
	bool b_IA_Inven_Allowed = true;
	/////////////////////////////  노트 ui /////////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_NoteUI;

	UFUNCTION()
	void NoteUIToggle(const FInputActionValue& Value);

	UFUNCTION()
	void SphereTraceFromCamera(); //상호작용 함수 

	UPROPERTY(VisibleAnywhere)
	float SphereRadius=30.f;
	UPROPERTY(VisibleAnywhere)
	int32 traceLength=500;

	UPROPERTY(BlueprintReadWrite, Category="Game Rules")
	bool b_IA_Note_Allowed = false;
	
	/////////// 지금 대화하고 있는 npc 가 누구인지 기억하기
	UPROPERTY(VisibleAnywhere)
	class ANPCCharacter* curNPC=nullptr;

	UPROPERTY(VisibleAnywhere)
	bool InteractHit =false;

	///////////// 지금 상호작용 대상을 알고있기
	UPROPERTY(VisibleAnywhere)
	AActor* Interact =nullptr;

	
	UPROPERTY(VisibleAnywhere)
	class AControllableLightActor* curControllableLight=nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Interact;

	void OnInteraction();
	
	// 델리게이트: 특정 장소에서만 추가될 Interaction 기능. 
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnSpecialInteraction OnSpecialInteraction;

	UPROPERTY()
	class ATestPlayerController* TestPC;

	UPROPERTY(VisibleAnywhere)
	bool IsLightRangeIn=false;

	////////////////자물쇠 관련 함수
	UFUNCTION()
	void HandleLockInteraction();

	// 블루프린트에서 구현할 함수 선언
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void HandleLockInteractionBP(AActor* TargetActor);

	//////////// 김영수 위대한 빨간 등대 선택시 1-3
public:
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArmComp2;
	
	// 전환용 카메라 부착해주기 
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArmComp;
	
	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* ChageCameracomp;
	
	//카메라 전환 함수
	void AdjustCameraPosition();

	FVector TargetCameraLocation; //타겟 위치
	FRotator TargetCameraRotation; //타겟 회전
	bool bIsCameraTransitioning; //카메라 전환 플래그
	float CameraTransitionSpeed; //속도

	float TargetFieldOfView; //줌
	float CurrentFieldOfView; //현재 줌 = 90도
	bool bIsFieldOfViewTransitioning;

	//UI애니메이션 종료 후 목으로 올라간 뒤에 사망
	UFUNCTION()
	void HangNeckUp();
	
	UFUNCTION()
	void UpdateMeshLocation();

	FVector TargetNeckLocation;
	FTimerHandle MoveMeshTimerHandle;

	////////////////// 소리 /////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundWave* NoteUIOpenSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundWave* NoteUICloseSound;
	
	UFUNCTION()
	void EndCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/////////////////////////2층 미션 수행 //////////////
	UPROPERTY(EditDefaultsOnly)
	class UArrowComponent* leftArrowComp;
	UPROPERTY(EditDefaultsOnly)
	class UArrowComponent* ItemArrowComp;
	UPROPERTY(EditDefaultsOnly)
	class UArrowComponent* CenterArrowComp;
	
	UPROPERTY(VisibleAnywhere)
	class AItemActor* curItem =nullptr;

	UPROPERTY(BlueprintReadWrite)
	bool bIsBeingAttacked=false;

	UPROPERTY(VisibleAnywhere)
	int32 Health=3;
	
	UFUNCTION()
	void DamagedToSecurity();

	UPROPERTY(EditDefaultsOnly)
	class UAudioComponent* AudioComp;

	void PlayHeartSound();
	
	void PlaySound(USoundWave* Sound);

	void StopSound();

	UPROPERTY(EditDefaultsOnly,Category=Sound)
	class USoundWave* HeartSound;

	UPROPERTY(EditDefaultsOnly,Category=Sound)
	class USoundWave* DamagedSound;

	///거미맵에서만 걷고 뛸때 소리가 들리도록 하기 ////////
	// 플레이어에게 소리 기능을 활성화하는 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Noise")
	FOnEnablePlayerNoise OnEnablePlayerNoise;
	
	//////////////////소리///////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundWave* NPCEventSoundWave;
	
	UPROPERTY(VisibleAnywhere) // 저장한 장소
	FString SaveLocationStr=FString(TEXT("2층 전시관 앞"));

	UPROPERTY(VisibleAnywhere) // 저장한 장소
	FTransform SaveTransform;

	UPROPERTY(EditDefaultsOnly,Category="Anim")
	class UAnimMontage* GetUp;
	UPROPERTY(EditDefaultsOnly,Category="Anim")
	class UAnimMontage* GetDown;
	UFUNCTION()
	void PlayGetUpAnimMontage();

	UFUNCTION(BlueprintCallable)
	void PlayGetDownAnimMontage();
	
	
};
	







