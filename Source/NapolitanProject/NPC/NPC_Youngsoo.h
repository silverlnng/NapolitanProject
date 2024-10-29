// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCCharacter.h"
#include "GameFramework/Character.h"
#include "NPC_Youngsoo.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ANPC_Youngsoo : public ANPCCharacter
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
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 NPC_ID =1;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 State = 1;

	virtual void Interact() override; 

	virtual int32 GetNPCID() override; //NPC ID를 들고엄

	virtual int32 GetState() override;

	virtual void ResultEvent(int32 result); //결과 함수

public:
	//0-1. 비명 소리
	UPROPERTY(EditAnywhere)
	class USoundBase* YSScreamSound;

	FTimerHandle TimerHandle;  // 타이머 핸들 선언

	//사라지는 효과
	bool bisDissolve = false;
	float dissolveAnimValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dissolve")
	UMaterialInterface* DissolveMaterial1;  // 블루프린트에서 지정할 머터리얼, 1-2
	
	UMaterialInstanceDynamic* DynamicMaterial;  // 동적 머터리얼 인스턴스

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dissolve")
	UMaterialInterface* DissolveMaterial2;  // 1-1

};
