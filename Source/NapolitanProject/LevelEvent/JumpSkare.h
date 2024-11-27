// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "JumpSkare.generated.h"

UCLASS()
class NAPOLITANPROJECT_API AJumpSkare : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJumpSkare();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UFUNCTION()
	void JumpSkareStart(); //점프스케어 발동

	bool bIsJumpSkare = false;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditAnywhere)
	float JumpSkareMaxSpeed=800.f;

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* CrawlMontage;
};
