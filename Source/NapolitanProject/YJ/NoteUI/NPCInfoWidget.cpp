// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCInfoWidget.h"

#include "Animation/WidgetAnimation.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"

void UNPCInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Img_Head->SetVisibility(ESlateVisibility::Hidden);
	Img_Head2->SetVisibility(ESlateVisibility::Hidden);
	Img_Head_BG->SetVisibility(ESlateVisibility::Hidden);

	GI=GetGameInstance<UMyTestGameInstance>();
}

void UNPCInfoWidget::LoadUpdate(const FString& str)
{
	// 수행한 퀘스트 에따라서 업데이트 시키기
	if (GI&&!GI->QuestCommandsMap.IsEmpty())
	{
		// 도슨트
		if (GI->QuestCommandsMap["DocentPlayEnd"].Done)
		{
			WidgetSwitcher_Docent1->SetActiveWidgetIndex(1);
		}

		// 청소부
		if (GI->QuestCommandsMap["CleanerQuestStart"].Done)
		{
			ScrollBox_Cleaner->ScrollWidgetIntoView(CanvasPanel_Cleaner1,true);
			WidgetSwitcher_Cleaner1->SetActiveWidgetIndex(1);
			PlayAnimation(Anim_Cleaner_1);
		}

		if (GI->QuestCommandsMap["CleanerQuestCompleted"].Done)
		{
			ScrollBox_Cleaner->ScrollWidgetIntoView(CanvasPanel_Cleaner2,true);
			WidgetSwitcher_Cleaner2->SetActiveWidgetIndex(1);
		}

		// 경비원
		if (GI->QuestCommandsMap["SecurityCompleted"].Done)
		{
			WidgetSwitcher_Security1->SetActiveWidgetIndex(1);
			Img_Head->SetVisibility(ESlateVisibility::Visible);
			Img_Head2->SetVisibility(ESlateVisibility::Visible);
			Img_Head_BG->SetVisibility(ESlateVisibility::Visible);
		}
		// 노인 
		
		// 나비
		
		if (GI->QuestCommandsMap["ButterflyQuestStart"].Done)
		{
			ScrollBox_Butterfly->ScrollWidgetIntoView(CanvasPanel_Butterfly_1,true);
			WidgetSwitcher_Butterfly_1->SetActiveWidgetIndex(1);
			WidgetSwitcher_Butterfly_2->SetActiveWidgetIndex(1);
		}

		if (GI->QuestCommandsMap["ButterflyQuestReward"].Done)
		{
			WidgetSwitcher_Butterfly_3->SetActiveWidgetIndex(1);
		}

		
	}
	
	
	if (str=="CleanerQuest")
	{
		ScrollBox_Cleaner->ScrollWidgetIntoView(CanvasPanel_Cleaner1,true);
		WidgetSwitcher_Cleaner1->SetActiveWidgetIndex(1);
		PlayAnimation(Anim_Cleaner_1);
	}
	else if (str=="CleanerQuestCompleted")
	{
		ScrollBox_Cleaner->ScrollWidgetIntoView(CanvasPanel_Cleaner2,true);
		WidgetSwitcher_Cleaner2->SetActiveWidgetIndex(1);
	}
	else if (str=="OldmanClue")
	{
		
	}
	else if (str=="ButterflyQuest")
	{
		WidgetSwitcher_Butterfly_1->SetActiveWidgetIndex(1);
		WidgetSwitcher_Butterfly_2->SetActiveWidgetIndex(1);
	}
	else if (str=="ButterflyQuestReward")
	{
		WidgetSwitcher_Butterfly_3->SetActiveWidgetIndex(1);
	}
	
}

void UNPCInfoWidget::SelectContent(int32 idx)
{
	WidgetSwitcher_->SetActiveWidgetIndex(idx);
}

void UNPCInfoWidget::SetForcus_ScrollBox_Cleaner(int32 panelNum,int32 wsNum)
{
	FTimerHandle UITimer1;

	GetWorld()->GetTimerManager().SetTimer(UITimer1,[this, panelNum]()
	{
		if (panelNum==1)
		{
			ScrollBox_Cleaner->ScrollWidgetIntoView(CanvasPanel_Cleaner1,true);
		}
		else if (panelNum==2)
		{
			ScrollBox_Cleaner->ScrollWidgetIntoView(CanvasPanel_Cleaner2,true);
		}
	},1.5f,false);
	

	FTimerHandle SoundTimer;
	GetWorld()->GetTimerManager().SetTimer(SoundTimer , [this]()
	{
		if (WriteSoundWave)
		{
			UGameplayStatics::PlaySound2D(this, WriteSoundWave);
		}	
	} , 1.8f , false);

	FTimerHandle UITimer2;

	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this, wsNum]()
	{
		if (wsNum==1)
		{
			WidgetSwitcher_Cleaner1->SetActiveWidgetIndex(1);
			PlayAnimation(Anim_Cleaner_1);
		}
		else if (wsNum==2)
		{
			WidgetSwitcher_Cleaner2->SetActiveWidgetIndex(1);
		}
		if (StickSoundWave)
		{
			UGameplayStatics::PlaySound2D(this, StickSoundWave);
		}
		
	},2.4f,false);
	
}

void UNPCInfoWidget::SetForcus_ScrollBox_Security(int32 panelNum, int32 wsNum)
{
	FTimerHandle UITimer1;

	GetWorld()->GetTimerManager().SetTimer(UITimer1,[this, panelNum]()
	{
		if (panelNum==1)
		{
			ScrollBox_Security->ScrollWidgetIntoView(CanvasPanel_Security1,true);
		}
		else if (panelNum==2)
		{
			ScrollBox_Security->ScrollWidgetIntoView(CanvasPanel_Security2,true);
		}
		
	},1.5f,false);

	FTimerHandle SoundTimer;
	GetWorld()->GetTimerManager().SetTimer(SoundTimer , [this]()
	{
		if (WriteSoundWave)
		{
			UGameplayStatics::PlaySound2D(this, WriteSoundWave);
		}	
	} , 1.8f , false);
	
	FTimerHandle UITimer2;

	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this, wsNum]()
	{
		if (wsNum==1)
		{
			WidgetSwitcher_Security1->SetActiveWidgetIndex(1);
			
			FTimerHandle UITimer3;
			GetWorld()->GetTimerManager().SetTimer(UITimer3 , [this]()
			{
				Img_Head->SetVisibility(ESlateVisibility::Visible);
				Img_Head2->SetVisibility(ESlateVisibility::Visible);
				Img_Head_BG->SetVisibility(ESlateVisibility::Visible);
			} , 0.5f , false);		

			
		}
		else if (wsNum==2)
		{
			
		}
		if (StickSoundWave)
		{
			UGameplayStatics::PlaySound2D(this, StickSoundWave);
		}
		
	},2.4f,false);
	
}

void UNPCInfoWidget::SetForcus_ScrollBox_Oldman(int32 panelNum, int32 wsNum)
{
	FTimerHandle UITimer1;

	GetWorld()->GetTimerManager().SetTimer(UITimer1,[this, panelNum]()
	{
		if (panelNum==1)
		{
			ScrollBox_Oldman->ScrollWidgetIntoView(CanvasPanel_Oldman1,true);
		}
		else if (panelNum==2)
		{
			ScrollBox_Oldman->ScrollWidgetIntoView(CanvasPanel_Oldman2,true);
		}
		if (WriteSoundWave)
		{
			UGameplayStatics::PlaySound2D(this, StickSoundWave);
		}
	},1.5f,false);
	
	FTimerHandle SoundTimer;
	GetWorld()->GetTimerManager().SetTimer(SoundTimer , [this]()
	{
		if (WriteSoundWave)
		{
			UGameplayStatics::PlaySound2D(this, WriteSoundWave);
		}	
	} , 1.8f , false);
	FTimerHandle UITimer2;

	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this, wsNum]()
	{
		if (wsNum==1)
		{
			WidgetSwitcher_Oldman1->SetActiveWidgetIndex(1);
		}
		else if (wsNum==2)
		{
			
		}
		if (StickSoundWave)
		{
			UGameplayStatics::PlaySound2D(this, StickSoundWave);
		}
		
	},2.4f,false);

	
}

void UNPCInfoWidget::SetForcus_ScrollBox_Docent(int32 panelNum, int32 wsNum)
{
	FTimerHandle UITimer1;

	GetWorld()->GetTimerManager().SetTimer(UITimer1,[this, panelNum]()
	{
		if (panelNum==1)
		{
			ScrollBox_Docent->ScrollWidgetIntoView(CanvasPanel_Docent1,true);
		}
		else if (panelNum==2)
		{
			ScrollBox_Docent->ScrollWidgetIntoView(CanvasPanel_Docent2,true);
		}
	},1.5f,false);

	FTimerHandle SoundTimer;
	GetWorld()->GetTimerManager().SetTimer(SoundTimer , [this]()
	{
		if (WriteSoundWave)
		{
			UGameplayStatics::PlaySound2D(this, WriteSoundWave);
		}	
	} , 1.8f , false);

	FTimerHandle UITimer2;

	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this, wsNum]()
	{
		if (wsNum==1)
		{
			WidgetSwitcher_Docent1->SetActiveWidgetIndex(1);
		}
		else if (wsNum==2)
		{
			
		}
		if (StickSoundWave)
		{
			UGameplayStatics::PlaySound2D(this, StickSoundWave);
		}
		
	},2.5f,false);

	
}

void UNPCInfoWidget::SetForcus_ScrollBox_Butterfly(int32 panelNum, int32 wsNum)
{
	// 먼저 판넬을 초점맞추고
	FTimerHandle UITimer1;

	GetWorld()->GetTimerManager().SetTimer(UITimer1,[this, panelNum]()
	{
		if (panelNum==1)
		{
			ScrollBox_Butterfly->ScrollWidgetIntoView(CanvasPanel_Butterfly_1,true);
		}
		else if (panelNum==2)
		{
			ScrollBox_Butterfly->ScrollWidgetIntoView(CanvasPanel_Butterfly_2,true);
		}
	},1.5f,false);
	
	FTimerHandle SoundTimer;
	GetWorld()->GetTimerManager().SetTimer(SoundTimer , [this]()
	{
		if (WriteSoundWave)
		{
			UGameplayStatics::PlaySound2D(this, WriteSoundWave);
		}	
	} , 1.8f , false);
	
	FTimerHandle UITimer2;

	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this, wsNum]()
	{
		if (wsNum==1)
		{
			WidgetSwitcher_Butterfly_1->SetActiveWidgetIndex(1);
			WidgetSwitcher_Butterfly_2->SetActiveWidgetIndex(1);
		}
		else if (wsNum==3)
		{
			WidgetSwitcher_Butterfly_3->SetActiveWidgetIndex(1);
		}
		if (StickSoundWave)
		{
			UGameplayStatics::PlaySound2D(this, StickSoundWave);
		}
	},2.5f,false);
	
}

// SetForcus_ScrollBox_Curator(2,1) => 두번째 캔버스로 이동시키고 위젯스위처_1 첫번째

void UNPCInfoWidget::SetForcus_ScrollBox_Curator(int32 panelNum, int32 wsNum)
{
	// ScrollWidgetIntoView 는 스크롤 박스 안에있는 위젯 중 매개변수안에 넣은 위젯으로 애니메이션 효과를 줌
	FTimerHandle UITimer1;
	GetWorld()->GetTimerManager().SetTimer(UITimer1,[this, panelNum]()
	{
		if (panelNum==1) // 스크롤이 첫번째 위젯쪽으로 이동 
		{
			ScrollBox_Curator->ScrollWidgetIntoView(CanvasPanel_Curator_1,true);
		}
		else if (panelNum==2) // 스크롤이 두번째 위젯쪽으로 이동 
		{
			ScrollBox_Curator->ScrollWidgetIntoView(CanvasPanel_Curator_2,true);
		}
	},1.5f,false);

	FTimerHandle SoundTimer;
	GetWorld()->GetTimerManager().SetTimer(SoundTimer , [this]()
	{
		if (WriteSoundWave)
		{
			UGameplayStatics::PlaySound2D(this, WriteSoundWave);
		}	
	} , 1.8f , false);

	FTimerHandle UITimer2;

	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this, wsNum]()
	{
		if (wsNum==1) // WidgetSwitcher 에서 맨처음 물음표(0) 값-> 두번째 ui (1) 으로 나오게 함
		{
			WidgetSwitcher_Curator_1->SetActiveWidgetIndex(1);
		}
		else if (wsNum==2)
		{
			
		}
		if (StickSoundWave)
		{
			UGameplayStatics::PlaySound2D(this, StickSoundWave);
		}
		
	},2.5f,false);
}

// SetForcus_ScrollBox_LeeSeo(1,1) (2,2) 

void UNPCInfoWidget::SetForcus_ScrollBox_LeeSeo(int32 panelNum, int32 wsNum)
{
	// ScrollWidgetIntoView 는 스크롤 박스 안에있는 위젯 중 매개변수안에 넣은 위젯으로 애니메이션 효과를 줌
	FTimerHandle UITimer1;
	GetWorld()->GetTimerManager().SetTimer(UITimer1,[this, panelNum]()
	{
		if (panelNum==1) // 스크롤이 첫번째 위젯쪽으로 이동 
		{
			ScrollBox_LeeSeo->ScrollWidgetIntoView(CanvasPanel_LeeSeo_1,true);
		}
		else if (panelNum==2) // 스크롤이 두번째 위젯쪽으로 이동 
		{
			ScrollBox_LeeSeo->ScrollWidgetIntoView(CanvasPanel_LeeSeo_2,true);
		}
	},1.5f,false);

	FTimerHandle SoundTimer;
	GetWorld()->GetTimerManager().SetTimer(SoundTimer , [this]()
	{
		if (WriteSoundWave)
		{
			UGameplayStatics::PlaySound2D(this, WriteSoundWave);
		}	
	} , 1.8f , false);

	FTimerHandle UITimer2;

	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this, wsNum]()
	{
		if (wsNum==1) // WidgetSwitcher 에서 맨처음 물음표(0) 값-> 두번째 ui (1) 으로 나오게 함
		{
			WidgetSwitcher_LeeSeo_1->SetActiveWidgetIndex(1);
		}
		else if (wsNum==2)
		{
			WidgetSwitcher_LeeSeo_2->SetActiveWidgetIndex(1);
		}
		if (StickSoundWave)
		{
			UGameplayStatics::PlaySound2D(this, StickSoundWave);
		}
		
	},2.5f,false);
}
