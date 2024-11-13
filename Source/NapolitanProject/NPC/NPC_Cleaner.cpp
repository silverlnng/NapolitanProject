// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Cleaner.h"

#include "AIController.h"
#include "NPCCleanerAnim.h"
#include "Components/CapsuleComponent.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/YJ/DialogueUI/NPCDialogueWidget.h"

// Sets default values
ANPC_Cleaner::ANPC_Cleaner()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HeadStaticMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadStaticMeshComp"));
	HeadStaticMesh->SetupAttachment(GetMesh(),"HeadSocket");

}

// Called when the game starts or when spawned
void ANPC_Cleaner::BeginPlay()
{
	Super::BeginPlay();

	AI = Cast<AAIController>(GetController());
	CleanerAnim = Cast<UNPCCleanerAnim>(GetMesh()->GetAnimInstance());
	

	HeadStaticMesh->SetHiddenInGame(true);

}

// Called every frame
void ANPC_Cleaner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//현재 상태 뜨게 하기
	FString myState = UEnum::GetValueAsString(mState);
	DrawDebugString(GetWorld() , GetActorLocation() , myState , nullptr , FColor::Yellow , 0 , true , 1);
	
	switch ( mState )
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
	case CleanerState::Stop:
		TickStop(DeltaTime);
	}

	//디졸브 이벤트
	if (bisDissolve)
	{
		dissolveAnimValue += DeltaTime / 4;

		// 원하는 범위 (0.5에서 -0.5)로 클램핑
		float DissolveValue1 = FMath::Clamp(0.5f - dissolveAnimValue, -0.5f, 0.5f);
		float DissolveValue2 = FMath::Clamp(0.5f - dissolveAnimValue, -0.5f, 0.5f);
		float DissolveValue3 = FMath::Clamp(0.5f - dissolveAnimValue, -0.5f, 0.5f);

		DynamicMaterial1->SetScalarParameterValue(TEXT("dissolve"), DissolveValue1);
		DynamicMaterial2->SetScalarParameterValue(TEXT("dissolve"), DissolveValue2);
		DynamicMaterial3->SetScalarParameterValue(TEXT("dissolve"), DissolveValue3);

		//UE_LOG(LogTemp, Error, TEXT("DissolveValue1: %f, DissolveValue2: %f"), DissolveValue1, DissolveValue2);
		
		if (DissolveValue1 <= -0.5f && DissolveValue2 <= -0.5f && DissolveValue3 <= -0.5f)
		{
			bItemSpawned = true;
			GetMesh()->SetVisibility(false);
			SpawnItems(); //아이템 스폰
			bisDissolve = false;
		}
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

void ANPC_Cleaner::ChangeCleared()
{
	Super::ChangeCleared();
}

void ANPC_Cleaner::TickIdle(const float& DeltaTime)
{
	//UE_LOG(LogTemp, Display, TEXT("Tick Idle"));
	//우선은 대기 상태 시간이 지나면 Move -> 특정 위치 도달시 청소
	CurrentTime += DeltaTime;
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

	// 새로운 타겟 포인트 로그 출력
	UE_LOG(LogTemp, Display, TEXT("New target point selected: X=%f, Y=%f, Z=%f"), targetPoint.X, targetPoint.Y, targetPoint.Z);

	//4. 선택된 위치로 이동 로직 추가
	if (AI)
	{
		AI->MoveToLocation(targetPoint);
		LastVisitedPoint = targetPoint; // 방문한 위치 업데이트
		//5. 만약 선택된 위치 근처에 왔다면
		if(FVector::Dist(GetActorLocation(), targetPoint) <= 300.f)
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
	//멈춤 상태로 변경
	
}


void ANPC_Cleaner::SetState(CleanerState newState)
{
	mState = newState;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("State changed to: %s"), *UEnum::GetValueAsString(mState)));
	//애니메이션 상태 지정
	CleanerAnim->animState = mState;
}

void ANPC_Cleaner::SpawnItems()
{
	//아이템 스폰
	if(bItemSpawned)
	{
		// 발끝 위치를 기준으로 스폰 위치 설정
		FVector FootLocation = GetMesh()->GetSocketLocation(FName("ItemSpawn"));
		FTransform SpawnTransform(FootLocation);

		// 블루프린트에서 설정된 SouvenirClass로 스폰, 청소부는 출력하는 아이템이 없음
		AActor* SouvenirActor = GetWorld()->SpawnActor<ASouvenirActor>(SouvenirClass, SpawnTransform );
		if (SouvenirActor)
		{
			//ItemActor->Tags.Add(FName("Item"));
			SouvenirActor->Tags.Add(FName("Souvenir"));
		}
	}

	bItemSpawned = false; //한번만 스폰되도록
}

// Called to bind functionality to input
void ANPC_Cleaner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ANPC_Cleaner::ResultEvent(int32 result)
{
	//이거 자체가 일단 호출되면 캐릭터의 움직임을 멈춤 상태로
	SetState(CleanerState::Stop);
	
	if (2==State)
	{
		if (0==result)
		{
			if (MainCharacter->curItem)
			{
				MainCharacter->curItem->PutDown();
			}
			
			HeadStaticMesh->SetHiddenInGame(false);
			
			// 스크립트 출력
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "ResultEvent");
			State=3;
			TestPC->StartEndNPCDialougue(true);
			TestPC->SetNPCDialougueText(0);

			//머터리얼 수정
			DynamicMaterial1 = UMaterialInstanceDynamic::Create(DissolveMaterial1, this);
			DynamicMaterial2 = UMaterialInstanceDynamic::Create(DissolveMaterial2, this);
			DynamicMaterial3 = UMaterialInstanceDynamic::Create(DissolveMaterial3, this);
			if (DynamicMaterial1 && DynamicMaterial2 && DynamicMaterial3)
			{
				GetMesh()->SetMaterial(0, DynamicMaterial1);
				GetMesh()->SetMaterial(1, DynamicMaterial2);
				GetMesh()->SetMaterial(2, DynamicMaterial3);
			}
			
			//디졸브
			GetWorldTimerManager().SetTimer(TimerHandle, [this]()
			{
				bisDissolve = true; //유품 스폰 뒤에 사라짐
				ChangeCleared(); //NPC 클리어
			}, 3.0f, false);
		}
	}
}

