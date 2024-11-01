// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Security.h"

void ANPC_Security::BeginPlay()
{
	Super::BeginPlay();

	// 라이트가 꺼져있다 : 랜덤한 위치으로 돌아다니기
		// 라이트가 꺼지고 랜덤으로 돌아다니 던 중 , 캐릭터 마주치면 캐릭터를 chase

	// 월드에있는 라이트가 켜져있는지 꺼져있는지 여부를 어떻게 확인 ??
	
	// 라이트가 켜져있으면 라이트를 향해 이동
		// 켜져있는 상태에서 캐릭터 마주치면 캐릭터를 우선순위로 삼아 chase

	// 돌아다닐때 거리에따라 소리
}

void ANPC_Security::SetState(ESecurityState curState)
{
	SecurityState=curState;
	
}
