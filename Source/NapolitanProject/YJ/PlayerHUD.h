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


	//===========�迵��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> NYSFactory;
	UPROPERTY()
	class UNYS_Choice* NYSUi; 
	
	
};
