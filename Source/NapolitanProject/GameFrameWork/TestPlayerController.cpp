// Fill out your copyright notice in the Description page of Project Settings.


#include "TestPlayerController.h"

#include "EventComponent.h"
#include "MyTestGameInstance.h"
#include "SaveGISubsystem.h"
#include "NapolitanProject/NPC/NPCCharacter.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/YJ/NoteUI/InventoryWidget.h"
#include "NapolitanProject/YJ/NoteUI/NoteWidget.h"
#include "NapolitanProject/YJ/NoteUI/SouvenirWidget.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "NapolitanProject/NapolitanProject.h"
#include "NapolitanProject/Interact/InteractWidget.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/YJ/DialogueUI/NPCDialogueWidget.h"
#include "NapolitanProject/YJ/DialogueUI/NPCResultWidget.h"
#include "NapolitanProject/YJ/NoteUI/ClueInfoWidget.h"

ATestPlayerController::ATestPlayerController()
{
	EventComponent = CreateDefaultSubobject<UEventComponent>(TEXT("EventComponent"));
}

void ATestPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ATestPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerHUD=GetHUD<APlayerHUD>();
	GI = GetGameInstance<UMyTestGameInstance>();
	SaveGI=GI->GetSubsystem<USaveGISubsystem>();

	me = Cast<ATestCharacter>(GetPawn());
	
	EventComponent->GI=GI;
	EventComponent->SaveGI=SaveGI;
	TWeakObjectPtr<ATestPlayerController> WeakThis = this;
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle,[&, WeakThis]()
	{
		if (!PlayerHUD)
		{
			UE_LOG(LogTemp,Warning,TEXT("PlayerHUD is null"));
			return;
		}
		if (!PlayerHUD->NoteUI)
		{
			UE_LOG(LogTemp,Warning,TEXT("PlayerHUD->NoteUI is null"));
			return;
		}
		if (!PlayerHUD->InventoryUI)
		{
			UE_LOG(LogTemp,Warning,TEXT("PlayerHUD->InventoryUI is null"));
			return;
		}
		
		// 유품
		SouvenirUI = PlayerHUD->NoteUI->WBP_Souvenir;
		SouvenirUI->SouvenirBtn_BackDele.AddDynamic(this , &ATestPlayerController::SetSouvenirUICurNumber);
		//초기값
		SetSouvenirUICurNumber(1);
		
	},1.0f,false);
	
}



void ATestPlayerController::SetUIMode(bool value)
{
	if (!value)
	{
		
		SetInputMode(FInputModeGameOnly());
		SetShowMouseCursor(false);
	}
	else
	{
		SetInputMode(FInputModeGameAndUI());
		SetShowMouseCursor(true);
	}
}

void ATestPlayerController::SetSouvenirUICurNumber(int curNum)
{
	//GI 의 SouvenirMap에서 앍어오기
	 // curNum 을 fname으로 바꾸기

	////////// 버튼 보이게,안보이게 처리
	if (1==curNum){SouvenirUI->Btn_Souvenir_Back->SetVisibility(ESlateVisibility::Hidden);}
	if (1!=curNum){SouvenirUI->Btn_Souvenir_Back->SetVisibility(ESlateVisibility::Visible);}
	if (SouvenirUI->MaxPage==curNum){SouvenirUI->Btn_Souvenir_front->SetVisibility(ESlateVisibility::Hidden);}
	if (SouvenirUI->MaxPage!=curNum){SouvenirUI->Btn_Souvenir_front->SetVisibility(ESlateVisibility::Visible);}
	//////////
	
	FName curNum_ = FName(FString::FromInt(curNum));
	if (!(GI->DT_SouvenirData)){return;}
	FSouvenirData* SouvenirData = GI->DT_SouvenirData->FindRow<FSouvenirData>(curNum_,TEXT(""));
	if (!SouvenirData){return;} // 방어코드

	// SouvenirData->IsHad 값에 따라서 
	if(!(SouvenirData->Had))
	{
		SouvenirUI->Img_Souvenir_left->SetColorAndOpacity(FLinearColor(0.01,0.01,0.01,1));
	}
	else
	{
		SouvenirUI->Img_Souvenir_left->SetColorAndOpacity(FLinearColor(1,1,1,1));
	}
	SouvenirUI->Img_Souvenir_left->SetBrushFromTexture(SouvenirData->thumnail);
	SouvenirUI->Text_Souvenir_left->SetText( FText::FromString(SouvenirData->SouvenirInfo));

	FName nextNum_ = FName(FString::FromInt(curNum+1));
	FSouvenirData* SouvenirDataNext = GI->DT_SouvenirData->FindRow<FSouvenirData>(nextNum_,TEXT(""));
	if (!SouvenirDataNext){return;} // 방어코드

	if(!SouvenirDataNext->Had)
	{
		SouvenirUI->Img_Souvenir_right->SetColorAndOpacity(FLinearColor(0.01,0.01,0.01,1));
	}
	else
	{
		SouvenirUI->Img_Souvenir_right->SetColorAndOpacity(FLinearColor(1,1,1,1));
	}
	
	SouvenirUI->Img_Souvenir_right->SetBrushFromTexture(SouvenirDataNext->thumnail);
	SouvenirUI->Text_Souvenir_right->SetText( FText::FromString(SouvenirDataNext->SouvenirInfo));
	
	
}

void ATestPlayerController::StartEndNPCDialougue(bool value)
{
	if (value) // 대화 시작할때 
	{
		SetUIMode(true); // ui 모드로 
		PlayerHUD->InteractUI->SetVisibleCrossHair(false);
		PlayerHUD->InteractUI->SetVisibleHBox(false);
		
		PlayerHUD->NPCDialogueUI->curOrder=0; // 초기화 작업 
		PlayerHUD->NPCDialogueUI->SetVisibility(ESlateVisibility::Visible);
		PlayerHUD->NPCDialogueUI->Text_Dialogue->SetText(FText::FromString(("")));
		PlayerHUD->NPCDialogueUI->Text_Name->SetText(FText::FromString(("")));
		//남아있는 텍스트 초기화
		
		int32 npcID =curNPC->GetNPCID();
		UE_LOG(LogTemp,Warning,TEXT("%s,npcID : %d"),*CALLINFO,npcID);
		int32 npcState =curNPC->GetState();

		// NPC 의 ID , STATE를 가져와서 
		SetCurNPCSelectUI(npcID,npcState,"kor");
		
		SetNPCDialougueMaxSize();
		
		CameraViewChangeNPC();

		//자신의 캐릭터는 안보이도록
		me->GetMesh()->SetHiddenInGame(true);
	}
	else // 대화 끝날때 
	{
		SetUIMode(false);
		PlayerHUD->NPCDialogueUI->SetVisibility(ESlateVisibility::Hidden);
		PlayerHUD->NPCDialogueUI->curOrder=0; // 초기화 작업
		me->SetPlayerState(EPlayerState::Idle);
		// 플레이어 상태 idle으로
		//SetViewTargetWithBlend(this,1.5f);
		CameraViewChangePlayer();

		//다시캐릭터보이게 -시간지연주고
		FTimerHandle MeshTimer;
		GetWorld()->GetTimerManager().SetTimer(MeshTimer,[this]()
		{
			me->GetMesh()->SetHiddenInGame(false);
		},1.5f,false);
	}
}

void ATestPlayerController::SetNPCDialougueMaxSize()
{
	PlayerHUD->NPCDialogueUI->MaxOrder=0; // 초기화 작업
	
	int32 npcID =curNPC->GetNPCID();
	int32 npcState =curNPC->GetState();
	int32 FindKey =(npcID*1000)+(npcState*100);
	for (int i=FindKey ; i<(FindKey+100); i++)
	{
		if (GI->NPCDialogueMap.Contains(i))
		{
			PlayerHUD->NPCDialogueUI->MaxOrder++;
		}
		else
		{
			break;
		}
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("MaxOrder: %d"),PlayerHUD->NPCDialogueUI->MaxOrder));
}

// 최대값 설정해줘야함 
void ATestPlayerController::SetNPCDialougueText(int32 curOrder)
{
	//curOrder 을 받아서 상황처리 하기 

	int32 npcID =curNPC->GetNPCID();
	
	int32 npcState =curNPC->GetState();
	
	int32 FindKey =(npcID*1000)+(npcState*100)+curOrder; // 시작하는 키값
	
	
	//UE_LOG(LogTemp,Warning,TEXT("%s,MaxOrder : %d"),*CALLINFO,PlayerHUD->NPCDialogueUI->MaxOrder);
	
	////////// 버튼 보이게,안보이게 처리
	if (0==curOrder){PlayerHUD->NPCDialogueUI->Btn_Back->SetVisibility(ESlateVisibility::Hidden);}
	if (0!=curOrder){PlayerHUD->NPCDialogueUI->Btn_Back->SetVisibility(ESlateVisibility::Visible);}
	if (PlayerHUD->NPCDialogueUI->MaxOrder!=(curOrder+1))
	{
		PlayerHUD->NPCDialogueUI->Btn_Next->SetVisibility(ESlateVisibility::Visible);
		// 선택창 나오도록 
		PlayerHUD->NPCDialogueUI->SetSelectSlotVisible(false);
	}
	if (PlayerHUD->NPCDialogueUI->MaxOrder==(curOrder+1))
	{
		PlayerHUD->NPCDialogueUI->Btn_Next->SetVisibility(ESlateVisibility::Hidden);

		FTimerHandle UITimer;
		GetWorld()->GetTimerManager().SetTimer(UITimer,[this]()
		{
			// 선택창이 있는 경우에만 선택창 나오도록 
			PlayerHUD->NPCDialogueUI->SetSelectSlotVisible(true);
		},1.5f,false);
	}
	
//////////////////////////////////////////////
	
	if (GI->NPCDialogueMap.Contains(FindKey)) // 있으면 출력하기 
	{
		FNPCDialogue Dialogue_=GI->NPCDialogueMap[FindKey];

		PlayerHUD->NPCDialogueUI->SetText_Dialogue(Dialogue_.Dialogue_Kor);
		
		PlayerHUD->NPCDialogueUI->SetText_Name(Dialogue_.Who);
		UE_LOG(LogTemp,Warning,TEXT("%s,%s"),*CALLINFO,*Dialogue_.Who);
		// 이벤트가 정의 되어있으면 이벤트를 발생시키기 
		// Dialogue_.CameraEffect 값에 따라서 이벤트를 실행시키기
		FString str=Dialogue_.EventNameString;
		if (!str.IsEmpty())
		{
			PlayerHUD->NPCDialogueUI->UIEffect(str);
			// UEventComponent 에 이벤트발생시키도록 전달
			EventComponent->StartEvent(str,Dialogue_.Dialogue_Kor);
			EventComponent->StartEvent_(str);
		}
	}
}

void ATestPlayerController::CameraViewChangeNPC()
{
	// npc 의 카메라로 전환 부드럽게 전환
	//
	if (curNPC)
	{
		//curNPC->CameraComp
		SetViewTargetWithBlend(curNPC,1.5f);
	}
	// 카메라의 이동을 lerp 하게 대화하는동안 움직이도록 하게
	 // GetWorldTimerManager().SetTimer(DialogueTimerHandle,this,)
}

void ATestPlayerController::CameraViewChangePlayer()
{
	//다시 원래 플레이어의 카메라로 전환하는 함수
	if (me)
	{
		SetViewTargetWithBlend(me, 1.5f);
	}
}

void ATestPlayerController::SetCurNPC(ANPCCharacter* curNPC_)
{
	curNPC = curNPC_;
}


int32 ATestPlayerController::SetNPCResultMaxSize(int32 selectedAnswer)
{
	PlayerHUD->NPCResultUI->MaxOrder=0; // 초기화 작업
	
	int32 npcID =curNPC->GetNPCID();
	int32 npcState =curNPC->GetState();

		
	int32 FindKey =(npcID*1000)+(npcState*100)+selectedAnswer*10;
	
	UE_LOG(LogTemp,Warning,TEXT("%s NPCResult FindKey :%d"),*CALLINFO,FindKey);
	
	for (int i=FindKey ; i<(FindKey+10); i++)
	{
		if (GI->NPCResultMap.Contains(i))
		{
			PlayerHUD->NPCResultUI->MaxOrder++;
		}
		else
		{
			break;
		}
	}
	
	return PlayerHUD->NPCResultUI->MaxOrder;
}

void ATestPlayerController::SetNPCResultText(int32 curOrder)
{
	if (!curNPC){return;}
	
	int32 npcID =curNPC->GetNPCID();
	
	int32 npcState =curNPC->GetState();

	int32 Answer=curNPC->GetResult();
	
	int32 FindKey =(npcID*1000)+(npcState*100)+(Answer*10)+curOrder;

	if (0==curOrder){PlayerHUD->NPCResultUI->Btn_Back->SetVisibility(ESlateVisibility::Hidden);}
	if (0!=curOrder){PlayerHUD->NPCResultUI->Btn_Back->SetVisibility(ESlateVisibility::Visible);}
	if (PlayerHUD->NPCResultUI->MaxOrder!=(curOrder+1))
	{
		PlayerHUD->NPCResultUI->Btn_Next->SetVisibility(ESlateVisibility::Visible);
	
	}
	// 마지막 대사 출력 
	if (PlayerHUD->NPCResultUI->MaxOrder==(curOrder+1))
	{
		PlayerHUD->NPCResultUI->Btn_Next->SetVisibility(ESlateVisibility::Hidden);

		// 마지막이므로 타이머로 닫도록 만들기
		FTimerHandle UITimer;

		GetWorld()->GetTimerManager().SetTimer(UITimer,[this]()
		{
			//닫으면 게임모드로 변경
			EndResult();
		},5.0f,false);
	}

	if (GI->NPCResultMap.Contains(FindKey)) // 있으면 출력하기 
	{
		FNPCResult NPCResult=GI->NPCResultMap[FindKey];

		PlayerHUD->NPCResultUI->SetText_Result(NPCResult.result_Kor);

		// 이벤트가 정의 되어있으면 이벤트를 발생시키기 
		// Dialogue_.CameraEffect 값에 따라서 이벤트를 실행시키기
		FString str=NPCResult.EventNameString;
		if (!str.IsEmpty())
		{
			// UEventComponent 에 이벤트발생시키도록 전달
			EventComponent->StartEvent(str,"");
			EventComponent->StartEvent_(str);
		}
	}
	
	
}

void ATestPlayerController::CallCurNPCResultEvent(int32 value)
{
	// 기존의 대화창 닫기 
	PlayerHUD->NPCDialogueUI->SetSelectSlotVisible(false);
	PlayerHUD->NPCDialogueUI->SetVisibility(ESlateVisibility::Hidden);
	UE_LOG(LogTemp,Warning,TEXT("%s NPCResult :%d"),*CALLINFO,value);
	
	if (curNPC)
	{
		curNPC->ResultEvent(value);
	}
	curNPC->SelectAnswer=value;
	// 결과 초기화 작업 => 미리 몇개인지 세고
	SetNPCResultMaxSize(value);
	UE_LOG(LogTemp,Warning,TEXT("%s,%d"),*CALLINFO,SetNPCResultMaxSize(value));
	
	if (SetNPCResultMaxSize(value)<=0){return;}
	// 결과대화가 있으면 나오도록 출력하기
	PlayerHUD->NPCResultUI->SetVisibility(ESlateVisibility::Visible);
	// 결과창 열고

	SetNPCResultText(0);
	
	// ui  닫기
}

void ATestPlayerController::EndResult()
{
	SetUIMode(false);
	PlayerHUD->NPCResultUI->SetVisibility(ESlateVisibility::Hidden);
	PlayerHUD->NPCResultUI->curOrder=0; // 초기화 작업
	me->SetPlayerState(EPlayerState::Idle);
	// 플레이어 상태 idle으로
	CameraViewChangePlayer();
}


void ATestPlayerController::SetCurNPCSelectUI(const int32& NPC_ID, const int32& State, const FString& Lang)
{
	int32 npc_id=NPC_ID;
	int32 npc_state=State;
	
	int32 FindKey =(npc_id*100)+(npc_state*10); // 시작하는 키값
	int32 count=0;
	TArray<FString> str;
	TArray<int32> result;

	// 110~115 까지 키값을 순회하기 => 선택지는 최대 5개까지니까 

	for (int i=FindKey ; i<=(FindKey+5); i++)
	{
		if (GI->NPCSelectMap.Contains(i)) // 있는 경우
		{
			// 위젯생성 하고 값 넣어주기 
			count++;
			const FNPCSelect& Select = GI->NPCSelectMap[i];
			result.Add(Select.Check);
			if (Lang == TEXT("kor"))
			{
				str.Add(*Select.Select_Kor);
			}
			else if(Lang == TEXT("eng"))
			{
				str.Add(*Select.Select_Eng);
			}
		}
		else // 없는경우
		{
			UE_LOG(LogTemp,Warning,TEXT("Select 를 찾을 수 없습니다."));
			break;
		}
	}
	SelectionSlotNum=count;
	// 한번호출
	PlayerHUD->NPCDialogueUI->CreateSelectionChildren(count,str,result);
}

void ATestPlayerController::SetCurNPCResultUI(int32 FindKey) // 한줄 출력용 
{
	if (GI->NPCResultMap.Contains(FindKey)) // 있는 경우
	{
		
		UE_LOG(LogTemp,Warning,TEXT("%s,%d"),*CALLINFO,FindKey);
		
		const FNPCResult& Result = GI->NPCResultMap[FindKey];
		if (GI->lang ==0)
		{
			// Select.Select_Kor;
			const FString str=Result.result_Kor;
			UE_LOG(LogTemp,Warning,TEXT("%s,%s"),*CALLINFO,*str);
			PlayerHUD->NPCDialogueUI->SetText_Dialogue(str);
		}
		else if(GI->lang ==1)
		{
			// Select.Select_Eng;
			const FString str=Result.result_Eng;
			UE_LOG(LogTemp,Warning,TEXT("%s,%s"),*CALLINFO,*str);
			PlayerHUD->NPCDialogueUI->SetText_Dialogue(str);
		}
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("%s,no result"),*CALLINFO);
	}
}

