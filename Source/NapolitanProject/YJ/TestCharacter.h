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

/////////////////// 달리기 기능 ////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Run;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=run)
	float runSpeed = StandingWalkSpeed*2.f;
	// 특정 키를 누르면 달리기
		// 누르고 있는 동안 && 쿨타임 내에서 
	float runCooltime =3.f;
	bool bIsRunning;
	bool bIsRunGageRemains;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=run)
	float RunGage =runCooltime ;
	
	void OnRunAction(const FInputActionValue& Value);

	void EndRunAction(const FInputActionValue& Value);

	void UpdateRunAction(float DeltaTime);
	void UpdateNotRunAction(float DeltaTime);
////////////////// 업드리기 기능 //////////////////////////
///
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	class UInputAction* IA_Crouched;

	void StartCrouch();
	void StopCrouch();
	void CrouchToggle(const FInputActionValue& Value);

	bool bCrouched;
	
	float StandingCapsuleHalfHeight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Crouch)
	float CrouchingCapsuleHalfHeight;

	float StandingCapsuleRadius;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Crouch)
	float CrouchingCapsuleRadius;
	
	float StandingWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Crouch)
	float CrouchingWalkSpeed;

/////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Tab;

	UFUNCTION()
	void NoteUIToggle(const FInputActionValue& Value);

	UFUNCTION()
	void SphereTraceFromCamera(); //상호작용 함수 

	UPROPERTY(VisibleAnywhere)
	float SphereRadius=30.f;
	UPROPERTY(VisibleAnywhere)
	int32 traceLength=500;

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

	UPROPERTY()
	class ATestPlayerController* TestPC;

	UPROPERTY(VisibleAnywhere)
	bool IsLightRangeIn=false;

	//////////// 김영수 위대한 빨간 등대 선택시 1-3
public:

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

////////////////// 소리 /////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundWave* NoteUIOpenSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundWave* NoteUICloseSound;

	UFUNCTION()
	void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult); // // 라이트 스위치 액터 등 가까이 있는 액터와 반응 할때 만든 함수

	UFUNCTION()
	void EndCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

/////////////////////////////////////////
	UPROPERTY(EditDefaultsOnly)
	class UArrowComponent* myArrowComp;
	
};







