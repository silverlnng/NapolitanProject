// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TestPlayerController.generated.h"

/**
 * 
 */



UCLASS()
class NAPOLITANPROJECT_API ATestPlayerController : public APlayerController
{
	GENERATED_BODY()
private:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
public:
	UPROPERTY(VisibleAnywhere)
	class UTestGameInstance* GI;
	UPROPERTY(VisibleAnywhere)
	class APlayerHUD* PlayerHUD;
	UPROPERTY(VisibleAnywhere)
	class USouvenirWidget* SouvenirUI;
	
	// SouvenirUI
	UFUNCTION()
	void SetSouvenirUICurNumber(int curNum);

	//////// 지금 대화하고 있는 npc 가 누구인지 기억하기
	UPROPERTY(VisibleAnywhere)
	class ANPCCharacter* curNPC=nullptr;
	
	UFUNCTION()
	void SetCurNPC(ANPCCharacter* curNPC_);

	UFUNCTION(BlueprintCallable)
	void SetNPCDialougueVisible(bool value);

	UFUNCTION()
	void SetNPCDialougueText(); // npc 상호작용 할때 딱한번 작동할 함수

	

	
	UFUNCTION()
	void CallCurNPCResultEvent(int32 value);

	UFUNCTION(BlueprintCallable)
	void SetCurNPCSelectUI(const int32& NPC_ID,const int32& State,const FString& Lang);

	
};

