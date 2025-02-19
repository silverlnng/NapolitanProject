// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RequestLetter.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API URequestLetter : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Ticket; //티켓 버튼을 누를시 시퀀스 발동 후 미술관 레벨로 이동 하는 코드

	UPROPERTY(EditDefaultsOnly)
	const TSoftObjectPtr<UWorld> MuseumLevel; //미술관 맵

	UFUNCTION()
	void OnTicketClicked();

	UFUNCTION()
	void OnLevelLoaded();

public:
	UPROPERTY(EditAnywhere, Category = "Cinematic")
	class ULevelSequence* OpeningSequence;

	UPROPERTY()
	class ATestCharacter* playerCharacter;

	UPROPERTY(VisibleAnywhere)
	class UMyTestGameInstance* GI;
	
};
