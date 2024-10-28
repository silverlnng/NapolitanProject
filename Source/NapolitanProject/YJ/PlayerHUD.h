// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UNoteWidget> NoteWidgetFactory;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	class UNoteWidget* NoteUI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UNPCDialogueWidget> NPCDialogueWidgetFactory;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	class UNPCDialogueWidget* NPCDialogueUI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UInteractWidget> InteractWidgetFactory;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	class UInteractWidget* InteractUI;

	//===========
	//사망 UI 생김
	UPROPERTY(EditDefaultsOnly)
	class UYSEvanceUI* YsEvanceUserWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UYSEvanceUI> YSEvanceFactory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UDeadEndingWidget> DeadEndingWidgetFactory;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	class UDeadEndingWidget* DeadEndingWidgetUI;
	
	
};
