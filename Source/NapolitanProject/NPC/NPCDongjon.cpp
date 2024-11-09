// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCDongjon.h"

#include "../GameFrameWork/TestPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SlateWrapperTypes.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/YJ/DialogueUI/NPCDialogueWidget.h"

// Sets default values
ANPCDongjon::ANPCDongjon()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANPCDongjon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPCDongjon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bisDissolve)
	{
		dissolveAnimValue += DeltaTime / 4;

		// 원하는 범위 (0.5에서 -0.5)로 클램핑
		float DissolveValue1 = FMath::Clamp(0.5f - dissolveAnimValue, -0.5f, 0.5f);
		float DissolveValue2 = FMath::Clamp(0.5f - dissolveAnimValue, -0.5f, 0.5f);
		float DissolveValue3 = FMath::Clamp(0.5f - dissolveAnimValue, -0.5f, 0.5f);
		float DissolveValue4 = FMath::Clamp(0.5f - dissolveAnimValue, -0.5f, 0.5f);

		DynamicMaterial1->SetScalarParameterValue(TEXT("dissolve"), DissolveValue1);
		DynamicMaterial2->SetScalarParameterValue(TEXT("dissolve"), DissolveValue2);
		DynamicMaterial3->SetScalarParameterValue(TEXT("dissolve"), DissolveValue3);
		DynamicMaterial4->SetScalarParameterValue(TEXT("dissolve"), DissolveValue4);

		UE_LOG(LogTemp, Error, TEXT("DissolveValue1: %f, DissolveValue2: %f"), DissolveValue1, DissolveValue2);

		IsCleared=true;
		GetComponentByClass<UCapsuleComponent>()->SetCollisionProfileName(FName("ClearedNPC"));

		if (DissolveValue1 <= -0.5f && DissolveValue2 <= -0.5f && DissolveValue3 <= -0.5f && DissolveValue4 <= -0.5f)
		{
			bisDissolve = false;
			GetMesh()->SetVisibility(false);
			bItemDropped = true;
		}
	}

}

// Called to bind functionality to input
void ANPCDongjon::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPCDongjon::Interact()
{
	Super::Interact();
}

int32 ANPCDongjon::GetNPCID()
{
	return NPC_ID;
}

int32 ANPCDongjon::GetState()
{
	return State;
}

void ANPCDongjon::ResultEvent(int32 result)
{
	// State 와 선택지의  result 에 따라 이벤트 정의하기
	if(1==State)
	{
		if(0 == result)
		{
			//플레이어 카메라 전환 -> 노인을 바라보도록 카메라를 다시 조정
			
			//“큐레이터에게 가보십시오. 분명 도움이 되리라.” 라는 대사
			//결과 대사 출력 부분
			int32 key=(NPC_ID*100)+(State*10)+result;
			PlayerHUD->NPCDialogueUI->SetVisibility(ESlateVisibility::Visible);
			TestPC->SetCurNPCResultUI(key);

			//머터리얼 수정
			DynamicMaterial1 = UMaterialInstanceDynamic::Create(DissolveMaterial1, this);
			DynamicMaterial2 = UMaterialInstanceDynamic::Create(DissolveMaterial2, this);
			DynamicMaterial3 = UMaterialInstanceDynamic::Create(DissolveMaterial3, this);
			DynamicMaterial4 = UMaterialInstanceDynamic::Create(DissolveMaterial4, this);
			if (DynamicMaterial1 && DynamicMaterial2 && DynamicMaterial3 && DynamicMaterial4)
			{
				GetMesh()->SetMaterial(0, DynamicMaterial1);
				GetMesh()->SetMaterial(1, DynamicMaterial2);
				GetMesh()->SetMaterial(2, DynamicMaterial3);
				GetMesh()->SetMaterial(3, DynamicMaterial4);
			}
			
			//디졸브
			GetWorldTimerManager().SetTimer(TimerHandle, [this]()
			{
				bisDissolve = true; //유품 스폰 뒤에 사라짐
				TestPC->StartEndNPCDialougue(false); //결과 출력
				if(bItemDropped)
				{
					//아이템 드랍
				}
			}, 4.0f, false);

			
			
		}
		else if(1 == result)
		{
			//노인과 대화 선택지
			int32 key=(NPC_ID*100)+(State*10)+result;
			PlayerHUD->NPCDialogueUI->SetVisibility(ESlateVisibility::Visible);
			TestPC->SetCurNPCResultUI(key);
			
		}
	}
}
