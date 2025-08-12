// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelMoveDoor_Curator.h"

#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/SaveGISubsystem.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/YJ/Monologue/MonolugueWidget.h"
#include "NapolitanProject/YJ/Save/GameSaveController.h"

void ALevelMoveDoor_Curator::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::BeginOverlap(OverlappedComponent , OtherActor , OtherComp , OtherBodyIndex , bFromSweep , SweepResult);
}

void ALevelMoveDoor_Curator::LevelMove()
{
	SaveGI->SetLevelMoveToDoor(true); //저장된 위치를 사용하라는 bool 변수 설정 함수
			
	// + 위치 적용 플래그 설정
	if (bMoveFromLobby) // 지금 현재레벨이 로비일때만 게임저장= 로비에서 다른레벨로 넘어갈때 실행하는것
	{
		//  지금 현재레벨이 로비일때만 이동하기전 위치를 저장해두기!!!!!!
		if (MainCharacter)
		{
			MainCharacter->SaveTransform=FromLevelLocComp->GetComponentTransform();
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("MyVector: %s"), *MainCharacter->SaveTransform.GetLocation().ToString()));
		}
		SaveGI->SavePlayerFTransform(FromLevelLocComp->GetComponentTransform());

		// 로드해야하는 주위 서브레벨들
		if (!SaveGI->SubLevelArray.IsEmpty())
		{
			SaveGI->SubLevelArray.Empty();
		}
		
		for (auto Level:SubLevelArray)
		{
			SaveGI->SubLevelArray.Add(Level);
		}
		
		SaveGI->GameSaveController->SaveGameToSlot(3); //여긴 자동저장 . 슬롯 3에 이전과 동일하도록 저장을 함 
		
		//레벨 이동하기 전에 타이머를 종료하고 가야 에러가 안남 !!!!!!!!
		if (GetWorld()->GetTimerManager().IsTimerActive(PlayerHUD->MonolugueWidgetUI->TextUpdateTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(PlayerHUD->MonolugueWidgetUI->TextUpdateTimerHandle);
		}
		if (GetWorld()->GetTimerManager().IsTimerActive(PlayerHUD->MonolugueWidgetUI->FinalTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(PlayerHUD->MonolugueWidgetUI->FinalTimerHandle);
		}
			
		//GI->AsyncLoadLoadLevel(MoveToLevel); //레벨 이동할때 로딩이 되도록
		// GI->PreloadLevel(MoveToLevelPathString);
		// 그림으로 레벨이동은 preload 를 안하고 해보기 
		
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			UGameplayStatics::OpenLevelBySoftObjectPtr(this,MoveToLevel,true); //레벨 변경
		}, 1.f, false);
	}
	else // 다른레벨에서 다시 로비로 돌아갈때 사용하는 것. 
	{
		FString SlotName = TEXT("SaveSlot_3");
		UTestSaveGame* LoadedGame = Cast<UTestSaveGame>(
			UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		
		// 큐레이터는 큐레이터와의 만남을 완료했는지 안했는지 검증을 해야함
		FName eventKey =TEXT("CuratorCompleted");
		if (GI->QuestCommandsMap.Contains(eventKey) && GI->QuestCommandsMap[eventKey].Done )
		{
			// 큐레이터와 만났으면 마지막 레벨로 이동을
			LoadedGame->QuestCommandsMap=GI->QuestCommandsMap;
			UGameplayStatics::SaveGameToSlot(LoadedGame , SlotName , 0);
			
			UGameplayStatics::OpenLevelBySoftObjectPtr(this,FinalLevel,true); //레벨 변경
		}
		else
		{
			if (LoadedGame)
			{
				SaveGI->GameSaveController->LoadGameFromSlot(3);
				// 여기에 오픈레벨(로비) 가 있음
			}
		}
	}
}
