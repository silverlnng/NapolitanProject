// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Cleaner.h"

#include "AIController.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/YJ/DialogueUI/NPCDialogueWidget.h"

// Sets default values
ANPC_Cleaner::ANPC_Cleaner()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANPC_Cleaner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPC_Cleaner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//현재 상태 뜨게 하기
	FString myState = UEnum::GetValueAsString(mState);
	DrawDebugString(GetWorld() , GetActorLocation() , myState , nullptr , FColor::Yellow , 0 , true , 1);
	
	switch (mState)
	{
	case CleanerState::Idle:
		TickIdle(DeltaTime);
		break;
	case CleanerState::Move:
		TickMove(DeltaTime);
		break;
	case CleanerState::Cleaning:
		TickCleaning(DeltaTime);
		break;
	}

}

void ANPC_Cleaner::Interact()
{
	Super::Interact();
}

int32 ANPC_Cleaner::GetNPCID()
{
	return NPC_ID;
}

int32 ANPC_Cleaner::GetState()
{
	// gi 의 ClearedNPC에서 경비원을 클리어했나 확인하기 
	if (!(GI->ClearedNPC.Contains(4)))
	{
		//클리어 못함
		State =1;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "State =1");
	}
	
	// 버튼눌렀으면 3으로 변경해야함 
	return State;
}

void ANPC_Cleaner::TickIdle(const float& DeltaTime)
{
	//우선은 대기 상태 시간이 지나면 Move -> 특정 위치 도달시 청소
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	if(CurrentTime > IdleDelayTime)
	{
		//이동 상태 전환
		//mState = CleanerState::Move;
		SetState(CleanerState::Move);
		CurrentTime = 0; //시간 초기화
	}
	
}

void ANPC_Cleaner::TickMove(const float& DeltaTime)
{
	//로직 : 우선 1~4 데이터 값 설정
	FVector dir = GetActorLocation();
	
	//1. 점들의 좌표를 배열에 추가
	TArray<FVector>points = {
		FVector(-470.0f, -1550.0f, 100.0f), //point1
		FVector(-470.0f, -4460.0f, 100.0f), //point2
		FVector(-2410.0f, -4460.0f, 100.0f), //point3
		FVector(-1950.0f, -1700.0f, 100.0f), //point4
		FVector(-1260.0f, -2890.0f, 100.0f) //point5
	};

	//2. 마지막 방문한 위치 제외하고 선택
	points.Remove(LastVisitedPoint);

	//3. 랜덤으로 하나의 점 선택
	int32 randomIndex = FMath::RandRange(0, points.Num() - 1);
	FVector targetPoint = points[randomIndex];

	//4. 선택된 위치로 이동 로직 추가
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->MoveToLocation(targetPoint);
		LastVisitedPoint = targetPoint; // 방문한 위치 업데이트
		//5. 만약 선택된 위치 근처에 왔다면
		if(dir.Size() <= 200.f)
		{
			SetState(CleanerState::Cleaning); //청소 상태 변경
		}
	}
	
}

void ANPC_Cleaner::TickCleaning(const float& DeltaTime)
{
	//멈춰서 청소 애니메이션 진행 -> 그다음 움직임 상태로 진행
	UE_LOG(LogTemp, Error, TEXT("TickCleaning"));
}
 
void ANPC_Cleaner::TickStop(const float& DeltaTime)
{
	//플레이어와 대화시 완전 정지 -> 대화가 끝났을 경우(1-0일 경우) 다시 Move로 변경
	
}

void ANPC_Cleaner::SetState(CleanerState newState)
{
	mState = newState;
	//애니메이션 상태 지정
}

// Called to bind functionality to input
void ANPC_Cleaner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ANPC_Cleaner::ResultEvent(int32 result)
{

	if (2==State)
	{
		if (0==result)
		{
			// -> 도슨트가 평범하게 다가오며 고개를 들어 당신을 올려다본다.==> 애니메이션
			// -> “게임을 해요! 도슨트의 넌센스 퀴즈!” ==>State 2 바로 시작하기
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "ResultEvent");
			State=3;
			TestPC->StartEndNPCDialougue(true);
			TestPC->SetNPCDialougueText(0);
		}
	}
	
	/*if(1==result)
	{
		if(0 == result)
		{
			//1-0. 그에게 이 미술관에 대해 묻는다.
			int32 key=(NPC_ID*100)+(State*10)+result;
			PlayerHUD->NPCDialogueUI->SetVisibility(ESlateVisibility::Visible);
			TestPC->SetCurNPCResultUI(key);
			
			
		}
		//플레이어가 청소부의 머리를 얻었을 경우 생기는 선택지 (버튼 visible로 처리해줘야힘)
		else if(1==result)
		{
			//1-0. 그에게 이 미술관에 대해 묻는다.
			int32 key=(NPC_ID*100)+(State*10)+result;
			PlayerHUD->NPCDialogueUI->SetVisibility(ESlateVisibility::Visible);
			TestPC->SetCurNPCResultUI(key);

			//유품 스폰 및 디졸브
		}
	}*/
}

