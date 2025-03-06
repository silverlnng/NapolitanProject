// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSpider_AnimInstance.h"

#include "AttackSpider.h"
#include "AttackSpiderV2.h"

void UAttackSpider_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	AttackSpider=Cast<AAttackSpiderV2>(TryGetPawnOwner());
}

void UAttackSpider_AnimInstance::AnimNotify_CheckAfterAttack()
{
	//AttackSpider->CheckAfterAttack();
}

void UAttackSpider_AnimInstance::AnimNotify_MoveToActor()
{
	//drop 애니 끝났을때 사용
	AttackSpider->CheckAttackRange();
	//이때 거리검사 해사 가까우면 바로 공격시작
	
	//AttackSpider->MoveToActor();
}

void UAttackSpider_AnimInstance::AnimNotify_AttackMiddle()
{
	
}

void UAttackSpider_AnimInstance::SetState(EAttackSpiderV2State State)
{
	AttackSpider->SetAIState(State);
}
