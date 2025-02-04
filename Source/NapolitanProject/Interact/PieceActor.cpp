// Fill out your copyright notice in the Description page of Project Settings.


#include "PieceActor.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"

void APieceActor::OnPickup()
{

	//Piece 조각은 인벤에 넣지말고 앞에 들고다니기 => 그래서 super 안함. 
	
	Picked=true;
	StaticMeshComp->SetRelativeRotation(FRotator(0,0,0));
	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,ECR_Ignore);
	AttachToComponent(MainCharacter->ItemArrowComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	// 소리 나오도록 하기 
	if (PickUpSoundWave)
	{
		UGameplayStatics::PlaySound2D(this, PickUpSoundWave);
	}
	
}
