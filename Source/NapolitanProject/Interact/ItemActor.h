// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NapolitanProject/Interface/InteractInterface.h"
#include "NapolitanProject/Interface/SaveAware.h"
#include "ItemActor.generated.h"



UCLASS()
class NAPOLITANPROJECT_API AItemActor : public AActor,public IInteractInterface, public ISaveAware
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class UMyTestGameInstance* GI;
	UPROPERTY()
	class USaveGISubsystem* SaveGI;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 ItemID=0;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;

	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface* M_Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundWave* ItemSoundWave;
	
//////언리얼 데이터 테이블 읽어오기 	//////////////////////////////////////////////////////////
	
	class ATestCharacter* MainCharacter;
	
	class ATestPlayerController* TestPC;
	
	UPROPERTY(VisibleAnywhere)
	class APlayerHUD* PlayerHUD;
	
	// 캐릭터가 상호작용 눌렀을때 실행시킬 함수
	virtual void IInteract() override;
	
	virtual void OnPickup(); // 아이템을 상호작용e키 눌렀을때 부를 함수 + 카메라 앞에 부착하기 

	virtual void Use();
	
	virtual void PutDown();

	virtual void OnInventorySlot();

	virtual void Remove();
	
	virtual int32 GetItemID();

	UPROPERTY(EditAnywhere)
	int32 AssociatedNpcID=-1;
	
	virtual int32 GetAssociatedNpcID() const override{return AssociatedNpcID;}
};
