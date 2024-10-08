// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NapolitanProject/NapolitanProjectCharacter.h"
#include "TestCharacter_YJ.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API ATestCharacter_YJ : public ACharacter
{
	GENERATED_BODY()
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
public:
	ATestCharacter_YJ();

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
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

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
	
};




