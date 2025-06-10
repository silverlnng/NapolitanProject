// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoundControlActor.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ASoundControlActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASoundControlActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp;

	UPROPERTY(EditDefaultsOnly)
	class UAudioComponent* AudioComp1; //1층-로비 배경음

	UPROPERTY(EditDefaultsOnly)
	class UAudioComponent* AudioComp2; //2층 배경음

	UPROPERTY(EditDefaultsOnly)
	class UAudioComponent* TextAudioComp2;

	class ATestCharacter* MainCharacter;
	class ATestPlayerController* TestPC;
	class APlayerHUD* PlayerHUD;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	bool bInLobby =true;

	
	UPROPERTY(EditDefaultsOnly,Category=SoundCue)
	class USoundCue* SecondFloor;
	UPROPERTY(EditDefaultsOnly,Category=SoundCue)
	class USoundCue* LobbyRoom;
	UPROPERTY(EditDefaultsOnly,Category=SoundCue)
	class USoundCue* DocentBG;
	
	UFUNCTION()
	void BGSoundChange(USoundCue* Sound);

	UFUNCTION()
	void SoundChangeOriginLobby();

	UFUNCTION()
	void TextSoundChange(bool value);
	
	UPROPERTY(EditDefaultsOnly)
	class USoundBase* textSound;
	
};

