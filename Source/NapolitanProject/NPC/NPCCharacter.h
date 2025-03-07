// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPCCharacter.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ANPCCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPCCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	int32 NPC_ID =-1;

	int32 State =-1;

	int32 SelectAnswer=-1;

	class ATestCharacter* MainCharacter;
	class ATestPlayerController* TestPC;
	class APlayerHUD* PlayerHUD;
	
	virtual int32 GetNPCID();

	virtual int32 GetState();

	virtual int32 GetResult(); 
	
	virtual void ResultEvent(int32 result); //결과 함수

	virtual void Interact();

	UPROPERTY(VisibleAnywhere)
	class UMyTestGameInstance* GI;
	
	// 전환용 카메라 부착해주기 
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArmComp;
	
	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* CameraComp;
	
	class UAnimMontage* TalkAnimMontage;
	
	virtual void playTalkAnimMontage();

//////////////////////////// 디졸브 효과
	bool bisDissolve = false;
	float dissolveAnimValue;

	UMaterialInstanceDynamic* DynamicMaterial;  // 동적 머터리얼 인스턴스
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dissolve")
	UMaterialInterface* DissolveMaterial_Yellow;  // 블루프린트에서 지정할 머터리얼, 1-2
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dissolve")
	UMaterialInterface* DissolveMaterial_Black; 

	virtual void DissolveEvent(FString& str);

	// 클리어하고 난뒤 상호작용 못하도록 
	bool IsCleared=false;
	virtual void ChangeCleared();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundWave* MissionClearSoundWave;
};

