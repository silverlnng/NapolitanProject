// Fill out your copyright notice in the Description page of Project Settings.


#include "Picture_LeeSeoYounger.h"

APicture_LeeSeoYounger::APicture_LeeSeoYounger()
{
	parentMeshComponent1 =CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("parentMeshComp1"));
	parentMeshComponent1->SetupAttachment(RootComponent);

	parentMeshComponent1 =CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("parentMeshComp12"));
	parentMeshComponent1->SetupAttachment(RootComponent);
}

void APicture_LeeSeoYounger::PlayBasicAnimMontage()
{
	Super::PlayBasicAnimMontage();
}

void APicture_LeeSeoYounger::PlayPaintAnimMontage()
{
	FRotator CurrentRotation = GetActorRotation();

	// Yaw 값을 -90도 감소 (오른쪽으로 회전) 
	CurrentRotation.Yaw -= 90.0f;
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

void APicture_LeeSeoYounger::PaintAnimEnd()
{
	StopAnimMontage(PaintAnimMontage);
	
	// 페인트 애니가 끝나고 다시  캐릭터를 회전 시키기
	PaintingCanvasStaticMeshComponent->SetRelativeLocation(FVector3d(72,0,0));
	PaintingCanvasStaticMeshComponent->SetRelativeRotation(FRotator(0,-270,0));
	
	// 캐릭터를 원래대로 정면으로 회전시키기 
	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Yaw += 90.0f;
	SetActorRotation(CurrentRotation);

	// 동생그림은 칭찬 받는 걸

	// 그리고 비웃도록 

	
		


	

	// 다음 애니메이션 실행 
}
