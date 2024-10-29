// Fill out your copyright notice in the Description page of Project Settings.


#include "YSEvanceUI.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/YJ/DeadEndingWidget.h"
#include "NapolitanProject/YJ/PlayerHUD.h"

void UYSEvanceUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 현재 게임의 HUD를 가져와 PlayerHUD로 캐스팅
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());
	}

	EndDelegate.Clear();
	EndDelegate.BindDynamic(this, &UYSEvanceUI::AddDeadEnding);
	BindToAnimationFinished(TextAnim, EndDelegate);
}

void UYSEvanceUI::PlayAnim()
{
	PlayAnimationForward(TextAnim);
}

void UYSEvanceUI::AddDeadEnding()
{
	// 끝나는 엔딩 위젯 나오도록 하기
	if (PlayerHUD && PlayerHUD->DeadEndingWidgetUI && PlayerHUD->YsEvanceUserWidget)
	{
		PlayerHUD->YsEvanceUserWidget->SetVisibility(ESlateVisibility::Hidden);
		PlayerHUD->DeadEndingWidgetUI->SetVisibility(ESlateVisibility::Visible);
		FString name= FString(TEXT("<Red_Big>남자</>"));
		PlayerHUD->DeadEndingWidgetUI->SetRichText_Name(name);
		PlayerHUD->DeadEndingWidgetUI->StartLerpTimer();
	}
}
