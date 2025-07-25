// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EventComponent.generated.h"

class IEventCommand;
// 이벤트를 정의하는 함수
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NAPOLITANPROJECT_API UEventComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEventComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void StartEvent_(FString& EventKey);

	FTimerHandle DelayTimerHandle;

	void CommandAdd();
	
	void UpdateText();
	
	FString CurrentText;
	FString FullText;
	FString RemoveTags;
	FString StartTag;
	FTimerHandle TextUpdateTimerHandle;

	UPROPERTY(VisibleAnywhere)
	class ATestPlayerController* TestPC;
	UPROPERTY(VisibleAnywhere)
	class APlayerHUD* PlayerHUD;
	UPROPERTY(VisibleAnywhere)
	class ATestCharacter* MainCharacter;
	UPROPERTY()
	class UMyTestGameInstance* GI;
	UPROPERTY()
	class USaveGISubsystem* SaveGI;

private:
	TMap<FString, TSharedPtr<IEventCommand>> CommandMap;
};
