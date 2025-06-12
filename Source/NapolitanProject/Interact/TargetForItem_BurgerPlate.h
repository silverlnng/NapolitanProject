// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetForItem.h"
#include "TargetForItem_BurgerPlate.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API ATargetForItem_BurgerPlate : public ATargetForItem
{
	GENERATED_BODY()
	ATargetForItem_BurgerPlate();
public:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp1;
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp2;
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp3;
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp4;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraComponent* NiagaraComp;
	
	virtual void CheckItem(class AItemActor* curItem) override;

	virtual void CheckItemSuccess()override;

	virtual void CheckItemFail()override;
	
	virtual void NoItem()override;

	void PutDown(int32 itemID,AItemActor* curItem);

	UPROPERTY(EditDefaultsOnly)
	TArray<int32> TargetItemIDArray={1,3,4,5};

	UPROPERTY(VisibleAnywhere)
	TArray<AItemActor*> PutDownitemID;
	
	void MissionCheck();

	class ANPC_Butterfly* NPC_Butterfly=nullptr;
	
	UPROPERTY()
	class UMyTestGameInstance* GI;
	UPROPERTY()
	class USaveGISubsystem* SaveGI;

public:	
	//////////////////소리///////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundWave* MissionClearSoundWave; //3개모두 다붙여서 미션 클리어했을때 소리
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundWave* OneTwoSoundWave; //1,2개 붙일때 소리 
	
};
