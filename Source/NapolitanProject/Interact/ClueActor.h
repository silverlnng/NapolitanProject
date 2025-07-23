// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NapolitanProject/Interface/InteractInterface.h"
#include "ClueActor.generated.h"



UCLASS()
class NAPOLITANPROJECT_API AClueActor : public AActor,public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClueActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	


	class ATestCharacter* MainCharacter;
	class ATestPlayerController* TestPC;
	class APlayerHUD* PlayerHUD;

	UPROPERTY(VisibleAnywhere)
	class UMyTestGameInstance* GI;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp;
	
	//  메쉬 와 콜리전
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;
	
	UPROPERTY(EditDefaultsOnly)
	int32 Clue_ID;

	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface* M_Overlay;
	
    // 캐릭터가 상호작용 눌렀을때 실행시킬 함수
	virtual void IInteract() override;
	void LookAt();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class URequestLetter> RequestLetterFactory;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	class URequestLetter* RequestLetterUI;
	
	UPROPERTY(VisibleAnywhere)
	TArray<FName> ClueDataRowNames;

	inline static int32 ClueActorCount=0;
};


