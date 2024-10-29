// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Youngsoo.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"
#include "NYS_Choice.h"
#include "TestPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "../YJ/TestCharacter.h"
#include "../YJ/PlayerHUD.h"

// Sets default values
ANPC_Youngsoo::ANPC_Youngsoo()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ANPC_Youngsoo::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ANPC_Youngsoo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (bisDissolve && DynamicMaterial)
	{
		dissolveAnimValue += DeltaTime / 4;

		// 원하는 범위 (0.5에서 -0.5)로 클램핑
		float DissolveValue = FMath::Clamp(0.5f - dissolveAnimValue, -0.5f, 0.5f);
		DynamicMaterial->SetScalarParameterValue(TEXT("dissolve"), DissolveValue);

		if (DissolveValue <= -0.5f)
		{
			bisDissolve = false;
			GetMesh()->SetVisibility(false);
		}
	}

}

// Called to bind functionality to input
void ANPC_Youngsoo::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPC_Youngsoo::Interact()
{
	Super::Interact();
}

int32 ANPC_Youngsoo::GetNPCID()
{
	return NPC_ID;
}

int32 ANPC_Youngsoo::GetState()
{
	return State;
}

void ANPC_Youngsoo::ResultEvent(int32 result)
{
	// State 와 선택지의  result 에 따라 이벤트 정의하기
	// 걸어가면서 사라짐 : 0 , 유품 획득 : 1, 에반스매듭 사망엔딩 : 2 

	if (1==State)
	{
		if (0 == result)
		{
			//남자를 위로해주자. “괜찮으십니까”의 경우 => 남자는 소리를 지르다가 사라짐
			UGameplayStatics::PlaySound2D(GetWorld(), YSScreamSound);
			// DynamicMaterial 생성 및 적용
			if (DissolveMaterial2)
			{
				DynamicMaterial = UMaterialInstanceDynamic::Create(DissolveMaterial2, this);
				if (DynamicMaterial)
				{
					GetMesh()->SetMaterial(0, DynamicMaterial);
				}
			}
			// 5초 후에 캐릭터를 숨기기 위한 타이머 설정
			GetWorldTimerManager().SetTimer(TimerHandle, [this]()
			{
				bisDissolve = true;
			}, 5.0f, false);
			
		}
		else if (1 == result)
		{
			// “단 한명만이 닻을 풀어 익사했다." => 내 가족들은... 이라는 대사
			// DynamicMaterial 생성 및 적용
			
			if (DissolveMaterial1)
			{
				DynamicMaterial = UMaterialInstanceDynamic::Create(DissolveMaterial1, this);
				if (DynamicMaterial)
				{
					GetMesh()->SetMaterial(0, DynamicMaterial);
				}
			}
			//유품 스폰
			
			GetWorldTimerManager().SetTimer(TimerHandle, [this]()
			{
				bisDissolve = true; //유품 스폰 뒤에 사라짐
			}, 6.0f, false);
			
			
		}
		else if (2 == result)
		{
			//남자에게 말을 걸어선 안된다. 어서 도망치자.의 경우 => 도망칠 경우 5초뒤에 NPC 사라짐

			GetWorldTimerManager().SetTimer(TimerHandle, [this]()
			{
				// 캐릭터의 Mesh를 보이지 않게 설정
				GetMesh()->SetVisibility(false);
			}, 5.0f, false);
		}
		else
		{
			TestPC->CameraViewChangePlayer();
			//위대한 빨간 등대를 부수자 -> 카메라 시점 변경 및 플레이어 얼굴을 클로즈 업
			MainCharacter->AdjustCameraPosition(); //시점 변경
			// 글자로 된 에반스 매듭이 나와서 플레이어 목 감싸고 피 터지는 연출과 함께 사망
			GetWorldTimerManager().SetTimer(TimerHandle, [this]()
			{
				PlayerHUD->CreateYSEvance(); //=교수형 UI 출력
			}, 7.0f, false);
		}
	}
	else if (2 == State)
	{
		
	}
	else if (3 == State)
	{
		
	}
	
}


