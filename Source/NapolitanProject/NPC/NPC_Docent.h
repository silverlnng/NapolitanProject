// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCCharacter.h"
#include "NPC_Docent.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API ANPC_Docent : public ANPCCharacter
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void ResultEvent(int32 result) override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 NPC_ID =2;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 State =1;
	
	virtual void Interact() override;

	virtual int32 GetNPCID() override;

	virtual int32 GetState() override;

	//유품
	UPROPERTY(EditAnywhere, Category = "Spawn Items")
	TSubclassOf<class ASouvenir_Docent> SouvenirNoteClass;
	
	UPROPERTY(EditDefaultsOnly,Category=Anim)
	class UAnimMontage* attackAnimMontage;

	UPROPERTY(EditDefaultsOnly,Category=Anim)
	class UAnimMontage* TalkAnimMontage;

	virtual void playTalkAnimMontage() override; 
	
	UFUNCTION()
	void UpdateLerp();
	FString name;
	FString SouvenirName;
	FTimerHandle LerpTimerHandle;
	float ElapsedTime = 0.0f;
	float LerpDuration = 10.0f;

///////////////// 디졸브 효과 ///////////////////	
	bool bisDissolve = false;
	float dissolveAnimValue;

	UMaterialInstanceDynamic* DynamicMaterial;  // 동적 머터리얼 인스턴스
	
	virtual void DissolveEvent(FString& str) override;
	virtual void ChangeCleared() override;

	UMaterialInstanceDynamic* DynamicMaterial1;  // 몸 머터리얼
	UMaterialInstanceDynamic* DynamicMaterial2;  // 목 머터리얼
	UMaterialInstanceDynamic* DynamicMaterial3;
	UMaterialInstanceDynamic* DynamicMaterial4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dissolve")
	UMaterialInterface* DissolveMaterial1;  // 블루프린트에서 지정할 머터리얼, 1-0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dissolve")
	UMaterialInterface* DissolveMaterial2;  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dissolve")
	UMaterialInterface* DissolveMaterial3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dissolve")
	UMaterialInterface* DissolveMaterial4;

///////// 사망이벤트 만들기
	UPROPERTY()
	class ALightControlActor* LightControlActor;
};


