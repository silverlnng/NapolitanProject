// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundClass.h"
#include "SettingWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API USettingWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

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
	
	UPROPERTY(meta = (BindWidget))
	class USlider* Slider_Total;

	UFUNCTION()
	void OnSlider_Total_Changed(float value);

	UPROPERTY(meta = (BindWidget))
	class USlider* Slider_BG;

	UFUNCTION()
	void OnSlider_BG_Changed(float value);

	UPROPERTY(meta = (BindWidget))
	class USlider* Slider_SFX;

	UFUNCTION()
	void OnSlider_SFX_Changed(float value);

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Setting;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Exit;

	UFUNCTION()
	void OnSlider_Setting_Changed(float value);

	//슬라이더 값을 움직일때 소리도 같이 해당 볼륨만큼 플레이 되도록 만들기
	class UMyTestGameInstance* GI;
	class USaveGISubsystem* SaveGI;
	
	UFUNCTION()
	void OnClickeSetting();
	UFUNCTION()
	void OnClickeExit();
};
