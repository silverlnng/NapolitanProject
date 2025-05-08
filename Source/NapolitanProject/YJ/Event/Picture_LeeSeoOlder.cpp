// Fill out your copyright notice in the Description page of Project Settings.


#include "Picture_LeeSeoOlder.h"

void APicture_LeeSeoOlder::PlayBasicAnimMontage()
{
	Super::PlayBasicAnimMontage();
	
}

void APicture_LeeSeoOlder::PlayPaintAnimMontage()
{
	FRotator CurrentRotation = GetActorRotation();

	// Yaw 값을 -90도 감소 (오른쪽으로 회전) 
	CurrentRotation.Yaw += 90.0f;

	// 새로운 회전값 설정
	SetActorRotation(CurrentRotation);

	// 붓 아이템을 보이도록 하기 
	AttachedStaticMeshComponent->SetHiddenInGame(false);

	// 그림 메쉬 보이도록 하기

	
	
	PaintingCanvasStaticMeshComponent->SetHiddenInGame(false);
	
	//BlueprintImplementableEventPlayPaintAnim();

	// 페인트 애니 작동시키기 
	if (AnimInstance&&PaintAnimMontage)
	{
		AnimInstance->Montage_Play(PaintAnimMontage);
	}
	
	// 페인트 애니가 끝나고 다시  캐릭터를 회전 시키기
	FTimerHandle NextAnimTimer1;
	GetWorld()->GetTimerManager().SetTimer(NextAnimTimer1,[this]()
	{
		PaintAnimEnd();
	},5.5f,false);
}

void APicture_LeeSeoOlder::PaintAnimEnd()
{
	PaintingCanvasStaticMeshComponent->SetRelativeLocation(FVector3d(-88,0,0));
	PaintingCanvasStaticMeshComponent->SetRelativeRotation(FRotator(0,90,0));
	StopAnimMontage(PaintAnimMontage);
	// 캐릭터를 원래대로 정면으로 회전시키기 
	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Yaw -= 90.0f;
	SetActorRotation(CurrentRotation);
	FTimerHandle NextAnimTimer1;
	
	// 그림도 정면으로 회전시키기 
		


	
	

	// 다음 애니메이션 실행 
	
}
