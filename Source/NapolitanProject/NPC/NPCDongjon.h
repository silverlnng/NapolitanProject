// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCCharacter.h"
#include "GameFramework/Character.h"
#include "NapolitanProject/Interact/ItemActor.h"
#include "NapolitanProject/Interact/Souvenir_Dongjun.h"
#include "NPCDongjon.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ANPCDongjon : public ANPCCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPCDongjon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void ResultEvent(int32 result) override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 NPC_ID =3; //고뇌하는 노인의 번호

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 State =1;
	
	virtual void Interact() override;

	virtual int32 GetNPCID() override;

	virtual int32 GetState() override;

	virtual void ChangeCleared() override; //사라지고 난 뒤 클리어

public:
	//사라지는 효과
	bool bisDissolve = false;
	float dissolveAnimValue;
	
	UMaterialInstanceDynamic* DynamicMaterial1;  // 동적 머터리얼 인스턴스
	UMaterialInstanceDynamic* DynamicMaterial2;  // 동적 머터리얼 인스턴스
	UMaterialInstanceDynamic* DynamicMaterial3;  // 동적 머터리얼 인스턴스
	UMaterialInstanceDynamic* DynamicMaterial4;  // 동적 머터리얼 인스턴스

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dissolve")
	UMaterialInterface* DissolveMaterial1;  // 블루프린트에서 지정할 머터리얼, 1-0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dissolve")
	UMaterialInterface* DissolveMaterial2;  // 1-1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dissolve")
	UMaterialInterface* DissolveMaterial3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dissolve")
	UMaterialInterface* DissolveMaterial4; 
	
	FTimerHandle TimerHandle;  // 타이머 핸들 선언

	//아이템, 유품을 스폰하는 함수
	void SpawnItems();

	bool bItemSpawned = false;
	bool bIsSpawn = false;
	
	UPROPERTY(EditAnywhere, Category = "Spawn Items")
	TSubclassOf<AItemActor> ItemClass;

	UPROPERTY(EditAnywhere, Category = "Spawn Items")
	TSubclassOf<ASouvenir_Dongjun> SouvenirClass;

};
