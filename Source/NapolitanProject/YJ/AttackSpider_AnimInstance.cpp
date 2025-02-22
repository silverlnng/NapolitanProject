// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSpider_AnimInstance.h"

#include "AttackSpider.h"

void UAttackSpider_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	AttackSpider=Cast<AAttackSpider>(TryGetPawnOwner());
}

void UAttackSpider_AnimInstance::AnimNotify_CheckAfterAttack()
{
	AttackSpider->CheckAttackRange();
}

void UAttackSpider_AnimInstance::AnimNotify_MoveToActor()
{
	AttackSpider->MoveToActor();
}
