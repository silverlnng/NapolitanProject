// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "YSEvanceUI.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UYSEvanceUI : public UUserWidget
{
	GENERATED_BODY()
public:
	// TextBlock을 에디터에서 바인딩할 수 있도록 UPROPERTY로 선언
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* EvanceText;

	// 전체 텍스트와 현재 상태를 저장하기 위한 변수들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Typing")
	FString TargetText;
    
	// 타이핑 효과를 위한 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Typing")
	float TypingSpeed = 0.1f;

public:
	virtual void NativeConstruct() override;

public:
	FString CurrentText;
	int32 CurrentIndex;
	FTimerHandle TypingTimerHandle;

	// 타이핑 업데이트 함수
	void UpdateText();
};
