// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCCharacter.h"
#include "GameFramework/Character.h"
#include "NapolitanProject/Interact/SouvenirActor.h"
#include "NPC_LeeSeo.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ANPC_LeeSeo : public ANPCCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC_LeeSeo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	virtual void ResultEvent(int32 result) override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 NPC_ID = 7; //최이서의 번호

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 State = 1;
	
	virtual void Interact() override;

	virtual int32 GetNPCID() override;

	virtual int32 GetState() override;

	virtual void ChangeCleared() override; //사라지고 난 뒤 클리어

public:
	//0-1. 비명 소리
	UPROPERTY(EditAnywhere)
	class USoundBase* ScreamSound;

	//맵에서 나오는 소름돋는 사운드
	UPROPERTY(EditAnywhere)
	class USoundBase* LSSound;

	//이서 점프스케어 시 재생되는 사운드
	UPROPERTY(EditAnywhere)
	class USoundBase* LSJump;
	
	//유품을 스폰하는 함수
	void SpawnItem();

	UPROPERTY(EditAnywhere, Category = "Spawn Items")
	TSubclassOf<ASouvenirActor> SouvenirClass;

	//유품 스폰 위치
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Point")
	TSubclassOf<AActor> LeeseoPoint;
	
	//점프스퀘어 발동 변수
	bool bJumpSquare = false;

	FTimerHandle JumpSquareTimerHandle;
	
	
};
