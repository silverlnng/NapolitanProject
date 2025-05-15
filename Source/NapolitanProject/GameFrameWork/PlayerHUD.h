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

	UPROPERTY(VisibleAnywhere)
	class UMyTestGameInstance* GI;
	
	class ATestPlayerController* PC;

	class ATestCharacter* MainCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UNoteWidget> NoteWidgetFactory;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	class UNoteWidget* NoteUI;
	UFUNCTION()
	void OnClickBtn_NoteClose();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundWave* CloseSoundWave;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UNPCDialogueWidget> NPCDialogueWidgetFactory;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	class UNPCDialogueWidget* NPCDialogueUI;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UNPCResultWidget> NPCResultWidgetFactory;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	class UNPCResultWidget* NPCResultUI;
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UInteractWidget> InteractWidgetFactory;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	class UInteractWidget* InteractUI;


	/////인벤토리 ui ///

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UInventoryWidget> InventoryFactory;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	class UInventoryWidget* InventoryUI;

	UFUNCTION()
	void OnClickBtn_InventoryClose();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundWave* InvenCloseSoundWave;
	
	//===========
	//사망 UI 생김
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UYSEvanceUI> YSEvanceFactory;
	UPROPERTY(EditDefaultsOnly)
	class UYSEvanceUI* YsEvanceUserWidget;

	UFUNCTION()
	void CreateYSEvance();
	
	//기본 DeadEndingUI
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UDeadEndingWidget> DeadEndingWidgetFactory;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	class UDeadEndingWidget* DeadEndingWidgetUI;


	// DeadEnd 에서 postprocess 효과 조절해야함
	UPROPERTY()
	class APostProcessVolume* AllPostProcessVolume;
	UPROPERTY(EditDefaultsOnly,Category = VignetteEffect) // 블프에서 할당하기, 
	UMaterialInterface* BaseMaterial;
	UPROPERTY()
	class UMaterialInstanceDynamic* PostProcessVignetteMatDynamic;

	UPROPERTY(EditDefaultsOnly,Category = VignetteEffect) // 블프에서 할당하기, 
	UMaterialInterface* RedVignetteMaterial;
	UPROPERTY()
	class UMaterialInstanceDynamic* RedVignetteMatDynamic;
	
	UFUNCTION()
	void PlayDeadVignetteEffect();
	UFUNCTION()
	void UpdateVignetteStrength();
	UFUNCTION()
	void PlayLevelLoadVignetteEffect();
	UFUNCTION()
	void UpdateMinusVignetteStrength();

	UFUNCTION()
	void PlayDamagedVignetteEffect();
	UFUNCTION()
	void UpdateRedVignetteStrength();
	
	UPROPERTY(EditDefaultsOnly, Category = VignetteEffect)
	float CurrentStrength = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = VignetteEffect)
	float TargetStrength = 200.0f;
	UPROPERTY(EditDefaultsOnly, Category = VignetteEffect)
	float LerpStep = 2.0f;        // 한 틱마다 얼마나 증가할지
	UPROPERTY(EditDefaultsOnly, Category = VignetteEffect)
	float TimerInterval = 0.01f;  // 0.01초마다 업데이트
	
	FTimerHandle VignetteTimerHandle;

		
	UPROPERTY(EditDefaultsOnly, Category = VignetteEffect)
	float Levelload_CurrentStrength = 200.0f;
	UPROPERTY(EditDefaultsOnly, Category = VignetteEffect)
	float Levelload_TargetStrength = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = VignetteEffect)
	float Levelload_LerpStep = 10.0f;        // 한 틱마다 얼마나 증가할지
	UPROPERTY(EditDefaultsOnly, Category = VignetteEffect)
	float Levelload_TimerInterval = 0.01f;

	UPROPERTY(EditDefaultsOnly, Category = RedVignetteEffect)
	float RedVignette_CurrentStrength = 2.3f;
	UPROPERTY(EditDefaultsOnly, Category = RedVignetteEffect)
	float RedVignette_TargetStrength = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = RedVignetteEffect)
	float RedVignette_LerpStep = 0.7f;        // 한 틱마다 얼마나 증가할지
	UPROPERTY(EditDefaultsOnly, Category = RedVignetteEffect)
	float RedVignette_TimerInterval = 0.01f; 

	FTimerHandle RedVignetteTimerHandle;
	
	//단서
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UClueSlotWidget> ClueSlotWidgetFactory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UMonolugueWidget> MonolugueWidgetFactory;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	class UMonolugueWidget* MonolugueWidgetUI;

	// 저장 ui
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class ULoadScreenWidget> LoadScreenWidgetFactory;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	class ULoadScreenWidget* LoadScreenUI;

	//ESC UI
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UESCWidget> UESCWidgetFactory;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	class UESCWidget* UESC_UI;
	
	UFUNCTION()
	void UpdateClueSlotWidget();

	UFUNCTION()
	void UpdateNPCInfoWidget();


};
