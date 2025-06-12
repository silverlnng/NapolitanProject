// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NapolitanProject/Interface/InteractInterface.h"
#include "TargetForItem.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ATargetForItem : public AActor,public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetForItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	class ATestCharacter* MainCharacter;
	class ATestPlayerController* TestPC;
	class APlayerHUD* PlayerHUD;
	
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* StaticMeshComp;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void CheckItem(class AItemActor* curItem);

	virtual void CheckItemSuccess();

	virtual void CheckItemFail();
	
	virtual void NoItem();

	// 캐릭터가 상호작용 눌렀을때 실행시킬 함수
	virtual void IInteract() override;
	
	// 상호작용하는 아이템에대해 미리 설정
	UPROPERTY(EditAnywhere)
	int32 TargetItemID=0;
};


