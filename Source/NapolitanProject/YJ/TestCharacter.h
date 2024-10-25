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
	Die
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
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
public:
	ATestCharacter();

protected:
	virtual void BeginPlay();
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
	//USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UPROPERTY(VisibleAnywhere)
	class APlayerController* PC;

	UPROPERTY(VisibleAnywhere)
	class APlayerHUD* PlayerHUD;
	
	UPROPERTY(VisibleAnywhere)
	EPlayerState curState=EPlayerState::Idle;

	UFUNCTION()
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

	//////// 지금 대화하고 있는 npc 가 누구인지 기억하기
	UPROPERTY(VisibleAnywhere)
	class ANPCCharacter* curNPC=nullptr;

	UPROPERTY(VisibleAnywhere)
	bool InteractHit =false;

	// 지금 상호작용 대상을 알고있기
	UPROPERTY(VisibleAnywhere)
	AActor* Interact =nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Interact;

	void OnInteraction();

	UPROPERTY()
	class ATestPlayerController* TestPC;
	
	//====== 김영수 위대한 빨간 등대 선택시 1-3
	//카메라와 메쉬 위치 조정에 필요한 변수들
public:
	FVector TargetCameraLocation;
	FRotator TargetCameraRotation;
	float CameraDistance = 500.f;
	float CameraLerpSpeed = 5.0f;
	bool bIsRedlighthouse = false;

public:
	//카메라 전환 함수
	void UpdateThirdPersonCamera(float DeltaTime);
	
};







