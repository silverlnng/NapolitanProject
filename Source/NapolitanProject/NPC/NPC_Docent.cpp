// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Docent.h"

#include "TestPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/Image.h"
#include "GameFramework/SpringArmComponent.h"
#include "NapolitanProject/YJ/DeadEndingWidget.h"
#include "NapolitanProject/YJ/InteractWidget.h"
#include "NapolitanProject/YJ/PlayerHUD.h"
#include "NapolitanProject/YJ/DialogueUI/NPCDialogueWidget.h"

void ANPC_Docent::ResultEvent(int32 result)
{
	// State 와 선택지의  result 에 따라 이벤트 정의하기
	// wait : 0 , Talk :1 ,ignore :2
	if (1==State)
	{
		if (0==result)
		{
			// -> 도슨트가 평범하게 다가오며 고개를 들어 당신을 올려다본다.==> 애니메이션
			// -> “게임을 해요! 도슨트의 넌센스 퀴즈!” ==>State 2 바로 시작하기
			State=2;
			TestPC->StartEndNPCDialougue(true);
			TestPC->SetNPCDialougueText(0);
		}
		else if (1==result)
		{
			int32 key=(NPC_ID*100)+(State*10)+result;
			PlayerHUD->NPCDialogueUI->SetVisibility(ESlateVisibility::Visible);
			TestPC->SetCurNPCResultUI(key);
			 // TestPC-> 키전달 c

			// 타이머 람다 로 PlayerHUD->NPCDialogueUI 안보이게 하고
	
			// 뷰전환이라면 npc 자신의 카메라를 향해
			
			FTimerHandle Timer;
			GetWorldTimerManager().SetTimer(Timer,[this]()
			{
				PlayerHUD->NPCDialogueUI->SetVisibility(ESlateVisibility::Hidden);
				//SpringArmComp->TargetArmLength=200.f;
				//SpringArmComp->SetRelativeLocation(FVector(0,0,40));
				//SpringArmComp->SetRelativeRotation(FRotator(0,180,0));
				//CameraComp->SetFieldOfView(20);
				PlayAnimMontage(attackAnimMontage);
			},3.0f,false);

			
			ElapsedTime = 0.0f;
			
			GetWorldTimerManager().SetTimer(LerpTimerHandle,this,&ANPC_Docent::UpdateLerp,0.01f, true,3.0f);
			
			// 메인캐릭터에게 다가오고 애니메이션 몽타주 실행
		
			
			
		}
		else if (2==result)
		{
		
		}
	}
	else if (2==State) //
	{
		if (0==result)
		{
			// 퀴즈에 대한 답 정의
			// 불태워죽인다
			TestPC->StartEndNPCDialougue(false);
			
		}
		else if (1==result)
		{
			// 찔러죽인다
		}
		else if (2==result)
		{
			// 삶아죽인다
		}
		else if (3==result)
		{
			// 튀겨죽인다
		}
		else if (4==result)
		{
			// 정답 . 만약 다른 선 택을 하게 되면 선택한 것에 따라 죽는다. 살아줘를 택할 시, 소년은 사라지고 그 자리에 수첩이 남는다. 
			TestPC->StartEndNPCDialougue(false);
			PlayerHUD->InteractUI->PlayNoteUIEvent(true);
		}
	}
	
}

void ANPC_Docent::UpdateLerp()
{
	ElapsedTime += 0.01f; // 타이머 호출 간격만큼 시간 증가

	// Lerp 비율 계산
	float Alpha = FMath::Clamp(ElapsedTime / LerpDuration, 0.0f, 1.0f);

	
	float Loc = FMath::Lerp(SpringArmComp->GetRelativeLocation().Z, 40, Alpha);
	float TargetArm = FMath::Lerp(SpringArmComp->TargetArmLength, 150, Alpha);
	float FieldOfView = FMath::Lerp(CameraComp->FieldOfView, 60, Alpha);
	
	SpringArmComp->SetRelativeLocation(FVector(0,0,Loc));
	SpringArmComp->TargetArmLength=TargetArm;
	CameraComp->SetFieldOfView(FieldOfView);
	
	if (Alpha >= 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(LerpTimerHandle);

		// 끝나는 엔딩 위젯 나오도록 하기
		PlayerHUD->DeadEndingWidgetUI->SetVisibility(ESlateVisibility::Visible);
	}
}


void ANPC_Docent::Interact()
{
	
}

int32 ANPC_Docent::GetNPCID()
{
	return NPC_ID;
}

int32 ANPC_Docent::GetState()
{
	return State;
}

