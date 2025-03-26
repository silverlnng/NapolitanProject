// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Docent.h"

#include "EngineUtils.h"
#include "../GameFrameWork/TestPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/Interact/Souvenir_Docent.h"
#include "NapolitanProject/LevelEvent/LightControlActor.h"


void ANPC_Docent::BeginPlay()
{
	Super::BeginPlay();
	
	for (TActorIterator<ALightControlActor> It(GetWorld(), ALightControlActor::StaticClass()); It; ++It)
	{
		LightControlActor = *It;
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
			
			// 1) 조명이벤트
			if (LightControlActor)
			{
				LightControlActor->StartSineFlicker(0,2,5);
			}

			// 아래에 피흐르는 나이아가라 스폰하고 자신은 안보이도록 하기

			
			
			
		}
		else if (2==result)
		{
			TestPC->StartEndNPCDialougue(false);
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
			TestPC->StartEndNPCDialougue(false);
		}
		else if (2==result)
		{
			// 삶아죽인다
			TestPC->StartEndNPCDialougue(false);
		}
		else if (3==result)
		{
			// 튀겨죽인다
			TestPC->StartEndNPCDialougue(false);
		}
		else if (4==result)
		{
			// 정답 . 만약 다른 선 택을 하게 되면 선택한 것에 따라 죽는다. 살아줘를 택할 시, 소년은 사라지고 그 자리에 수첩이 남는다. 
			TestPC->StartEndNPCDialougue(false);
			
			FTimerHandle Timer;
			GetWorldTimerManager().SetTimer(Timer,[this]()
			{
				//SouvenirName= FString(TEXT("수첩"));
				//PlayerHUD->InteractUI->GetSouvenirEvent(SouvenirName);
				//PlayerHUD->InteractUI->PlayNoteUIEvent(true);

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
			
			// 미술관을 탐색하자 퀘스트 발생 시키기 => 유품 쪽에서
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


