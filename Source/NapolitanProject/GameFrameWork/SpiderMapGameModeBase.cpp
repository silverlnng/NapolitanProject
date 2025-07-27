// Fill out your copyright notice in the Description page of Project Settings.


#include "SpiderMapGameModeBase.h"

#include "EngineUtils.h"
#include "MyTestGameInstance.h"
#include "PlayerHUD.h"
#include "SaveGISubsystem.h"
#include "TestCharacter.h"
#include "TestPlayerController.h"
#include "Components/ArrowComponent.h"
#include "Components/Border.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/NapolitanProject.h"
#include "NapolitanProject/Interact/CatchSpider.h"
#include "NapolitanProject/Interact/InteractWidget.h"
#include "NapolitanProject/Interact/ItemActor.h"
#include "NapolitanProject/YJ/AttackSpiderV2.h"
#include "NapolitanProject/YJ/SpiderMapGunActor.h"
#include "NapolitanProject/YJ/NoteUI/InventoryWidget.h"
#include "NapolitanProject/YJ/Save/TestSaveGame.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"


ASpiderMapGameModeBase::ASpiderMapGameModeBase()
{
	// 경로 대입 하기
	static ConstructorHelpers::FObjectFinder<USoundMix> SFXMix_(TEXT("/Game/Resource/Audio/SoundClass/SFXMix.SFXMix"));
	if (SFXMix_.Succeeded())
	{
		SFXMix=SFXMix_.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<USoundClass> SFXClass_Finder(TEXT("/Game/Resource/Audio/SoundClass/SC_SFX.SC_SFX"));
	if (SFXClass_Finder.Succeeded())
	{
		// 임시 U포인터 사용하여 형변환 수행
		USoundClass* TempSoundClass = SFXClass_Finder.Object;
		MySFXClass=TempSoundClass;
	}

	///Script/Engine.SoundMix'/Game/Resource/Audio/SoundClass/MasterMix.MasterMix'
	static ConstructorHelpers::FObjectFinder<USoundMix> MasterMix_Finder(TEXT("/Game/Resource/Audio/SoundClass/MasterMix.MasterMix"));
	if (MasterMix_Finder.Succeeded())
	{
		MasterMix=MasterMix_Finder.Object;
	}
	///Script/Engine.SoundClass'/Game/Resource/Audio/SoundClass/SC_Master.SC_Master'
	static ConstructorHelpers::FObjectFinder<USoundClass> MyMasterClass_Finder(TEXT("/Game/Resource/Audio/SoundClass/SC_Master.SC_Master"));
	if (MyMasterClass_Finder.Succeeded())
	{
		MyMasterClass=MyMasterClass_Finder.Object;
	}
	// /Script/Engine.SoundMix'/Game/Resource/Audio/SoundClass/BackGroundMix.BackGroundMix'
	static ConstructorHelpers::FObjectFinder<USoundMix> BackGroundMix_Finder(TEXT("/Game/Resource/Audio/SoundClass/BackGroundMix.BackGroundMix"));
	if (BackGroundMix_Finder.Succeeded())
	{
		BackGroundMix=BackGroundMix_Finder.Object;
	}
	///Script/Engine.SoundClass'/Game/Resource/Audio/SoundClass/SC_BG.SC_BG'
	static ConstructorHelpers::FObjectFinder<USoundClass> BackGroundClass_Finder(TEXT("/Game/Resource/Audio/SoundClass/SC_BG.SC_BG"));
	if (BackGroundClass_Finder.Succeeded())
	{
		BackGroundClass=BackGroundClass_Finder.Object;
	}
}

void ASpiderMapGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	MainCharacter=Cast<ATestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!MainCharacter){return;}
	PC =MainCharacter->GetController<ATestPlayerController>();
	if (!PC){return;}
	PlayerHUD =PC->GetHUD<APlayerHUD>();
	
	
	//MainCharacter->GetMesh()->SetWorldScale3D(FVector3d(1,1,1));
		
	for (TActorIterator<AAttackSpiderV2> It(GetWorld(), AAttackSpiderV2::StaticClass()); It; ++It)
	{
		AttackSpiderV2 = *It;
		
	}
	
	if (MainCharacter)
	{
		// 기존 바인딩 제거
		MainCharacter->OnSpecialInteraction.Clear();

		// 새로운 기능 바인딩
		MainCharacter->OnSpecialInteraction.AddDynamic(this, &ASpiderMapGameModeBase::Interaction_OnSpiderMap);

		MainCharacter->OnEnablePlayerNoise.AddDynamic(this,&ASpiderMapGameModeBase::MakeNoisePlayer);
		
		// 거미맵에서만 총 부착시키기 
		// MainCharacter

		if (GunBP)
		{
			Gun = GetWorld()->SpawnActor<ASpiderMapGunActor>(GunBP);
		}
		if (Gun)
		{
				Gun->BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,ECR_Ignore);
					
				Gun->AttachToComponent(MainCharacter->leftArrowComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			
		}

		//거미맵에서만 걸을때 들리도록하기 하기
		
	}
	if (SpiderBP)
	{
		//SpiderItem=SpiderBP->GetDefaultObject<AItemActor>();
		SpiderItem = GetWorld()->SpawnActor<AItemActor>(SpiderBP);
	}

	
	GI =GetGameInstance<UMyTestGameInstance>();
	SaveGI=GI->GetSubsystem<USaveGISubsystem>();
	if (GI) // 로드플레이 아닐때도 해야함
	{
		// 시간 지연을 주기
		FTimerHandle GITimer;
		GetWorld()->GetTimerManager().SetTimer(GITimer,[this]()
		{
			UE_LOG(LogTemp,Warning,TEXT("%sLoadedGame CatchSpider :%s"),*CALLINFO,*SaveGI->CatchSpiderNum);
			
			SaveGI->RestoreAttachedItems();
			
			CatchSpiderCount=FCString::Atoi(*SaveGI->CatchSpiderNum);
		},0.5f,false);
		
		if (!SaveGI->ClearedNPC.IsEmpty())
		{
			for (int32 key :SaveGI->ClearedNPC)
			{
				if (2==key) // 도슨트
				{
					MainCharacter->b_IA_Note_Allowed = true;
					FTimerHandle HUDTimer;
					GetWorld()->GetTimerManager().SetTimer(HUDTimer , [this]()
					{
						PlayerHUD->InteractUI->Border_Note->SetVisibility(ESlateVisibility::Visible);
					} , 1.5f , false);
				}
			}
		}
		
		LoadSettingValue();
	}
	
}

void ASpiderMapGameModeBase::Interaction_OnSpiderMap(AActor* Interact)
{
	// 현재 거미잡이 총을 잡고있고 거미를 향해 E키를 눌렀을경우4
	// MainCharacter->curItem->ItemID

	
	ACatchSpider* CatchSpider =Cast<ACatchSpider>(Interact);
	
	if (CatchSpider)
	{
		Gun->Fired();
		MakeNoisePlayer();
		
		CatchSpider->Health--;
		if (CatchSpider->Health>0)
		{
			CatchSpider->Damaged();
		}
		else if (CatchSpider->Health==0)
		{
			CatchSpider->Die();
			// 갯수도 누적해서 표시.
			CatchSpiderCount++;
			// 잡을때 마다 늘어난 숫자를 저장시키기 
			SaveGI->CatchSpiderNum=FString::FromInt(CatchSpiderCount);
			
			FString CatchSpiderNum=FString::FromInt(CatchSpiderCount);

			if (CatchSpiderCount==1)
			{
				SpiderItem->BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,ECR_Ignore);
				SpiderItem->AttachToComponent(MainCharacter->ItemArrowComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			
				SpiderItem->SetActorHiddenInGame(true);

				SaveGI->SavedItems.Add(SpiderItem->GetClass());
			
				if (PlayerHUD->InventoryUI)
				{
					if (PlayerHUD->InventoryUI->InvenSlots.Contains(SpiderItemID))
					{
						PlayerHUD->InventoryUI->InvenSlots[SpiderItemID]->OnItemAcquired();
						// 아이템 슬롯 작업.
						PlayerHUD->InventoryUI->InvenSlots[SpiderItemID]->Set_TextNum(CatchSpiderNum);
					}
				}
				// 해당하는 아이템슬롯을 찾아서 자신을 넣어주기
				if (PlayerHUD->InventoryUI->InvenSlots.Contains(SpiderItemID))
				{
					PlayerHUD->InventoryUI->InvenSlots[SpiderItemID]->MyItem=SpiderItem;
				}

				// 인벤 효과 애니메이션 실행시키기 
				PlayerHUD->InteractUI->PlayInvenUIEvent();
				
				int32 ItemRow =SpiderItemID;
				
				if (GI->itemDataMap.Contains(ItemRow))
				{
					GI->itemDataMap[ItemRow].Had=true;
					FString ItemName=GI->itemDataMap[ItemRow].Name;
					PlayerHUD->InteractUI->GetItemEvent(ItemName); 
				}
				
			}
			else
			{
				// 인벤 효과 애니메이션 실행시키기 
				PlayerHUD->InteractUI->PlayInvenUIEvent();
				PlayerHUD->InventoryUI->InvenSlots[SpiderItemID]->Set_TextNum(CatchSpiderNum);
			}
		}
	}
}

void ASpiderMapGameModeBase::MakeNoisePlayer()
{
	//MainCharacter->GetActorLocation() 에서 z값만 증가시켜서
	FVector origin=MainCharacter->GetActorLocation();
	// 높이만 증가
	if (AttackSpiderV2)
	{
		origin.Z=AttackSpiderV2->GetActorLocation().Z;
	}

	MakeNoise(100.f,MainCharacter,origin,NoiseRange);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("플레이어의 소리 발생")));
}

void ASpiderMapGameModeBase::LoadSettingValue()
{
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(),MasterMix,MyMasterClass,SaveGI->TotalSoundMix,1,0);
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(),BackGroundMix,BackGroundClass,SaveGI->BGSoundMix,1,0);
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(),SFXMix,MySFXClass,SaveGI->SFXSoundMix,1,0);
}

void ASpiderMapGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	MainCharacter->OnSpecialInteraction.Clear();

	MainCharacter->OnEnablePlayerNoise.Clear();

	FString SlotName = TEXT("SaveSlot_3");
	UTestSaveGame* LoadedGame = Cast<UTestSaveGame>(
		UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	
	if (LoadedGame)
	{
		LoadedGame->SavedItems=SaveGI->SavedItems;
		LoadedGame->CatchSpiderNum=SaveGI->CatchSpiderNum;
		UGameplayStatics::SaveGameToSlot(LoadedGame , SlotName , 0);
	}

	

	// 총 부착-해제시키기
	Gun->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
}

