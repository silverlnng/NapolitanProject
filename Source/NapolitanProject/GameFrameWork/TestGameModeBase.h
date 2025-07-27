// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NapolitanProject/YJ/Save/TestSaveGame.h"
#include "TestGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API ATestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ATestGameModeBase();

	virtual void StartPlay() override;
	
	virtual void BeginPlay() override;

	
	class ATestPlayerController* PC;
		
	class ATestCharacter* MainCharacter;
	
	class APlayerHUD* PlayerHUD;

	UPROPERTY()
	class UMyTestGameInstance* GI;
	UPROPERTY()
	class USaveGISubsystem* SaveGI;

	UPROPERTY(VisibleAnywhere)
	TMap<int32,class ANPCCharacter*> NPCArray;

	UPROPERTY(VisibleAnywhere)
	TMap<int32,class AItemActor*> ItemActorArray;

	UPROPERTY(EditInstanceOnly)
	class AItemActor* BreadItem4;
	UPROPERTY(EditInstanceOnly)
	class AItemActor* BreadItem5;

	// 삭제해야하는 Monologue 박스 
	UPROPERTY(EditAnywhere) // 맵에서 할당하기 
	class AMonologueTriggerBox* MonologueBox3=nullptr;

	UPROPERTY(EditAnywhere) // 맵에서 할당하기 
	class AMonologueTriggerBox* MonologueBox4=nullptr;

	UPROPERTY(EditAnywhere)
	class ASouvenir_Docent* Souvenir_Docent=nullptr;
	
	UPROPERTY()
	TSet<TSoftObjectPtr<UWorld>> SubLevelArray;

	UPROPERTY(EditDefaultsOnly,Category="Sound")
	class  USoundMix* SFXMix;

	UPROPERTY(EditDefaultsOnly,Category="Sound")
	class USoundClass* MySFXClass;

	UPROPERTY(EditDefaultsOnly,Category="Sound")
	class  USoundMix* MasterMix;

	UPROPERTY(EditDefaultsOnly,Category="Sound")
	class USoundClass* MyMasterClass;

	UPROPERTY(EditDefaultsOnly,Category="Sound")
	class  USoundMix* BackGroundMix;

	UPROPERTY(EditDefaultsOnly,Category="Sound")
	class USoundClass* BackGroundClass;
	
	void LoadSettingValue();
	
};
