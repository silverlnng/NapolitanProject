// Fill out your copyright notice in the Description page of Project Settings.


#include "PieceActor.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"

void APieceActor::OnPickup()
{
	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,ECR_Ignore);
	//Piece 조각은 인벤에 넣지말고 앞에 들고다니기
	AttachToComponent(MainCharacter->ItemArrowComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}
