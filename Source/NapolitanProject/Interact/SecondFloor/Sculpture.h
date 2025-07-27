// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NapolitanProject/Interface/SaveAware.h"
#include "Sculpture.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ASculpture : public AActor, public ISaveAware
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASculpture();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class ATestCharacter* MainCharacter;
	class ATestPlayerController* TestPC;
	class APlayerHUD* PlayerHUD;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp;
	
	//  메쉬 와 콜리전
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* SkeletalMeshComp;
	
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp1;
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp2;
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp3;

	// 나이아가라 컴포넌트 
	UPROPERTY(EditDefaultsOnly)
	class UNiagaraComponent* NiagaraComp;
	UPROPERTY(EditDefaultsOnly)
	class UNiagaraComponent* NiagaraComp2;
	UPROPERTY(EditDefaultsOnly)
	class UNiagaraComponent* NiagaraComp3;
	
	// 아이템을 가진상태에서 아이템을 내려놓을수있도록 
	void PutDownPiece(class APieceActor* Piece);

	UPROPERTY(VisibleAnywhere)
	TArray<APieceActor*> PieceArray;

	// 아이템 3개가 모두 모였는지 확인하는 함수
	void MissionCheck();

	UPROPERTY(VisibleAnywhere)
	class ANPC_Security* NPC_Security;

	// 단서 제공하기
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AClueActor> Clue10;

	//////////////////소리///////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundWave* MissionClearSoundWave; //3개모두 다붙여서 미션 클리어했을때 소리
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundWave* OneTwoSoundWave; //1,2개 붙일때 소리
	
	UPROPERTY()
	class UMyTestGameInstance* GI;
	UPROPERTY()
	class USaveGISubsystem* SaveGI;
	
	UPROPERTY(EditAnywhere)
	int32 AssociatedNpcID=4;
	virtual int32 GetAssociatedNpcID() const override{return AssociatedNpcID;}
	
};
