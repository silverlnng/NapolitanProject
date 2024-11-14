// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Youngsoo.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"
#include "NYS_Choice.h"
#include "../GameFrameWork/TestPlayerController.h"
#include "YoungsooAnim.h"
#include "Kismet/GameplayStatics.h"
#include "../GameFrameWork/TestCharacter.h"
#include "../GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/Interact/InteractWidget.h"
#include "NapolitanProject/YJ/DialogueUI/NPCDialogueWidget.h"

// Sets default values
ANPC_Youngsoo::ANPC_Youngsoo()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	anim = Cast<UYoungsooAnim>(GetMesh()->GetAnimInstance());
}

// Called when the game starts or when spawned
void ANPC_Youngsoo::BeginPlay()
{
	Super::BeginPlay();

	//GetMesh()->SetVisibility(false);
}

// Called every frame
void ANPC_Youngsoo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
			//남자를 위로해주자. “괜찮으십니까”의 경우 => 남자는 소리를 지르다가 사라지고, 물 나이아가라 발생
			int32 key=(NPC_ID*100)+(State*10)+result;
			PlayerHUD->NPCDialogueUI->SetVisibility(ESlateVisibility::Visible);
			TestPC->SetCurNPCResultUI(key);
			
			//애니메이션 블루프린트 실행
			anim = Cast<UYoungsooAnim>(GetMesh()->GetAnimInstance());
			anim->playYSSadMontage(); //슬퍼하는 애니메이션 작동
			UGameplayStatics::PlaySound2D(GetWorld(), YSScreamSound);

			TestPC->StartEndNPCDialougue(false);
			
			// 5초 후에 캐릭터를 숨기기 위한 타이머 설정
			GetWorldTimerManager().SetTimer(TimerHandle, [this]()
			{
				FString dissolve = "Black";
				DissolveEvent(dissolve);
				ChangeCleared();
			}, 4.0f, false);
			
		}
		else if (1 == result)
		{
			// “단 한명만이 닻을 풀어 익사했다." => 내 가족들은... 이라는 대사
			// DynamicMaterial 생성 및 적용
			int32 key=(NPC_ID*100)+(State*10)+result;
			PlayerHUD->NPCDialogueUI->SetVisibility(ESlateVisibility::Visible);
			TestPC->SetCurNPCResultUI(key);
			
			//유품 스폰

			//캐릭터 사라짐
			GetWorldTimerManager().SetTimer(TimerHandle, [this]()
			{
				FString dissolve = "Yellow";
				DissolveEvent(dissolve); //디졸브 이벤트 발생
				TestPC->StartEndNPCDialougue(false); //결과 대화창 출력
				ChangeCleared();
			}, 4.0f, false);

		
			//아이템 획득
			FTimerHandle SouvenirTimer;
			GetWorldTimerManager().SetTimer(SouvenirTimer, [this]()
			{
				FString SouvenirName= FString(TEXT("파란 스카프를"));
				PlayerHUD->InteractUI->GetSouvenirEvent(SouvenirName);
				ChangeCleared();
			}, 8.0f, false);

			
			FName NPC_IDName = FName(*FString::FromInt(NPC_ID));
			
			FSouvenirData* SouvenirData= GI->DT_SouvenirData->FindRow<FSouvenirData>(NPC_IDName , TEXT(""));
			if (SouvenirData)
			{
				SouvenirData->Had=true;
			}
			
			TestPC->SetSouvenirUICurNumber(NPC_ID);
		}
		else if (2 == result)
		{
			//남자에게 말을 걸어선 안된다. 어서 도망치자.의 경우 => 도망칠 경우 5초뒤에 NPC 사라짐

			GetWorldTimerManager().SetTimer(TimerHandle, [this]()
			{
				// 캐릭터의 Mesh를 보이지 않게 설정
				GetMesh()->SetVisibility(false);
				ChangeCleared();
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

void ANPC_Youngsoo::ChangeCleared()
{
	Super::ChangeCleared();
}

void ANPC_Youngsoo::DissolveEvent(FString& str)
{
	Super::DissolveEvent(str);
}


