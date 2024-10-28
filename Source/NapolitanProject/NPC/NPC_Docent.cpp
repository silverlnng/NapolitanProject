// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Docent.h"

#include "TestPlayerController.h"

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

		}
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
