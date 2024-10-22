// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Five.h"

void ANPC_Five::ResultEvent(int32 result)
{
	// 여기서 result 에 따라서 다르게 정의하기 
	
}

void ANPC_Five::Interact()
{
	// 대화창 시작하기 
	// 자신의 NPC_ID , State 으로 데이터 가져와서 알맞는 대사가  ui 에 출력되도록
}

int32 ANPC_Five::GetNPCID()
{
	return NPC_ID;
}

int32 ANPC_Five::GetState()
{
	return State;
}
