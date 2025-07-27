// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Docent.h"

#include "EngineUtils.h"
#include "../GameFrameWork/TestPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/Interact/Souvenir/Souvenir_Docent.h"
#include "NapolitanProject/LevelEvent/TriggerCustom/EventTriggerBox_Docent.h"
#include "NapolitanProject/LevelEvent/LightControlActor.h"


void ANPC_Docent::BeginPlay()
{
	Super::BeginPlay();
	
	for (TActorIterator<ALightControlActor> It(GetWorld(), ALightControlActor::StaticClass()); It; ++It)
	{
		LightControlActor = *It;
	}

	for (TActorIterator<AEventTriggerBox_Docent> It(GetWorld(), AEventTriggerBox_Docent::StaticClass()); It; ++It)
	{
		EventTriggerBox_Docent = *It;
	}
}

void ANPC_Docent::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bisDissolve)
	{
		dissolveAnimValue += DeltaSeconds / 4;

		// 원하는 범위 (0.5에서 -0.5)로 클램핑
		float DissolveValue1 = FMath::Clamp(0.5f - dissolveAnimValue, -0.5f, 0.5f);
		float DissolveValue2 = FMath::Clamp(0.5f - dissolveAnimValue, -0.5f, 0.5f);
		float DissolveValue3 = FMath::Clamp(0.5f - dissolveAnimValue, -0.5f, 0.5f);
		float DissolveValue4 = FMath::Clamp(0.5f - dissolveAnimValue, -0.5f, 0.5f);
		
		DynamicMaterial1->SetScalarParameterValue(TEXT("dissolve"), DissolveValue1);
		DynamicMaterial2->SetScalarParameterValue(TEXT("dissolve"), DissolveValue2);
		DynamicMaterial3->SetScalarParameterValue(TEXT("dissolve"), DissolveValue3);
		DynamicMaterial4->SetScalarParameterValue(TEXT("dissolve"), DissolveValue4);
		
		
	}
}

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
			TestPC->StartEndNPCDialougue(false);
			// 사망이벤트를 만들기

			// 배경음 변경하기
			if (ScarySound)
			{
				UGameplayStatics::PlaySound2D(this, ScarySound);
			}
			
			// 1) 조명이벤트-> 깜빡이다가 쭉 어두워지게
			if (LightControlActor)
			{
				LightControlActor->StartSineFlicker(0,2,3,5.5f,10.f);
			}

			PlayOnce();
			
			// 아래에 피흐르는 나이아가라 스폰하고 도슨트 안보이도록 하기, 상호작용 안되도록 .
			GetComponentByClass<UCapsuleComponent>()->SetCollisionProfileName(FName("ClearedNPC"));

			FTimerHandle HiddenTimer;

			GetWorld()->GetTimerManager().SetTimer(HiddenTimer,[this]()
			{
				SetActorHiddenInGame(true);
			},1.0f,false);
			
			
			
			// NPCEventManage에 도슨트 점프스케어 1 추가
			// 아님 트리거박스가 실행되도록 델리게이트 바인드.=> AEventTriggerBox_Docent를 월드에서 찾아야함.
			if (EventTriggerBox_Docent)
			{
				EventTriggerBox_Docent->BindBeginOverlap();
			}

			// 점프스케어 용 도슨트를 활성화 시키기
			
		}
		else if (2==result)
		{
			TestPC->StartEndNPCDialougue(false);
		}
	}
	else if (2==State)
	{
		if (0==result)
		{
			// 퀴즈에 대한 답 정의
			// 먹을 것을 준다.
			TestPC->StartEndNPCDialougue(false);
			
		}
		else if (1==result)
		{
			// 밖으로 나가자고 한다.
			TestPC->StartEndNPCDialougue(false);
		}
		else if (2==result)
		{
			// 동정심을 표한다.
			TestPC->StartEndNPCDialougue(false);
		}
		else if (3==result)
		{
			// 정답 . 만약 다른 선 택을 하게 되면 선택한 것에 따라 죽는다. 
			TestPC->StartEndNPCDialougue(false);
			
			FTimerHandle Timer;
			GetWorldTimerManager().SetTimer(Timer,[this]()
			{
				
				//몸 머터리얼 수정
				DynamicMaterial1 = UMaterialInstanceDynamic::Create(DissolveMaterial1 , this);
				DynamicMaterial2 = UMaterialInstanceDynamic::Create(DissolveMaterial2 , this);
				DynamicMaterial3 = UMaterialInstanceDynamic::Create(DissolveMaterial3 , this);
				DynamicMaterial4 = UMaterialInstanceDynamic::Create(DissolveMaterial4 , this);


				if (DynamicMaterial1 && DynamicMaterial2 && DynamicMaterial3&&DynamicMaterial4)
				{
					GetMesh()->SetMaterial(0 , DynamicMaterial1);
					GetMesh()->SetMaterial(1 , DynamicMaterial2);
					GetMesh()->SetMaterial(2 , DynamicMaterial3);
					GetMesh()->SetMaterial(3 , DynamicMaterial4);
				}
				
				bisDissolve = true;
			} , 1.5f , false);

			
			// 수첩유품을 생성시키기
			FTimerHandle UITimer2;

			GetWorld()->GetTimerManager().SetTimer(UITimer2 , [this]()
			{
				FVector FootLocation = GetMesh()->GetSocketLocation(FName("ItemSpawn"));
				FTransform SpawnTransform(FootLocation);

				ASouvenir_Docent* SouvenirActor = GetWorld()->SpawnActor<ASouvenir_Docent>(
					SouvenirNoteClass , SpawnTransform);
			} , 7.0f , false);
			
			
			FTimerHandle UITimer1;

			GetWorld()->GetTimerManager().SetTimer(UITimer1,[this]()
			{
				MainCharacter->b_IA_Note_Allowed = true;
				ChangeCleared(); 
			},6.0f,false);

			if (MissionClearSoundWave)
			{
				UGameplayStatics::PlaySound2D(this, MissionClearSoundWave);
			}
		}
	}
	
}

void ANPC_Docent::playTalkAnimMontage()
{
	if (TalkAnimMontage)
	{
		//PlayAnimMontage(TalkAnimMontage);
	}
}

void ANPC_Docent::UpdateLerp()
{
	ElapsedTime += 0.01f; // 타이머 호출 간격만큼 시간 증가

	// Lerp 비율 계산
	float Alpha = FMath::Clamp(ElapsedTime / LerpDuration, 0.0f, 1.0f);

	
	float Loc = FMath::Lerp(SpringArmComp->GetRelativeLocation().Z, 40, Alpha);
	float TargetArm = FMath::Lerp(SpringArmComp->TargetArmLength, 160, Alpha);
	float FieldOfView = FMath::Lerp(CameraComp->FieldOfView, 80, Alpha);
	
	SpringArmComp->SetRelativeLocation(FVector(0,0,Loc));
	SpringArmComp->TargetArmLength=TargetArm;
	CameraComp->SetFieldOfView(FieldOfView);
	
	if (Alpha >= 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(LerpTimerHandle);
	}
}

void ANPC_Docent::DissolveEvent(FString& str)
{
	Super::DissolveEvent(str);
}

void ANPC_Docent::ChangeCleared()
{
	Super::ChangeCleared();
}

void ANPC_Docent::PlayOnce()
{
	if (PlayGhostAnimation)
	{
		GetMesh()->PlayAnimation(PlayGhostAnimation, false); // false = 한 번만 실행
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


