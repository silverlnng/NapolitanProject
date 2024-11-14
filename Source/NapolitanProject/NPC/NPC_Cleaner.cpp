// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Cleaner.h"

#include "AIController.h"
#include "NPCCleanerAnim.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/NapolitanProject.h"
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

	bIsMoving = false;

	// 맵에 있는 모든 BP_Points 액터 찾기
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PointClass, BP_PointsArray);
	
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
		break;
	}

	//디졸브 이벤트
	if (bisDissolve)
	{
		dissolveAnimValue += DeltaTime / 4;

		// 원하는 범위 (0.5에서 -0.5)로 클램핑
		float DissolveValue1 = FMath::Clamp(0.5f - dissolveAnimValue, -0.5f, 0.5f);
		float DissolveValue2 = FMath::Clamp(0.5f - dissolveAnimValue, -0.5f, 0.5f);
		float DissolveValue3 = FMath::Clamp(0.5f - dissolveAnimValue, -0.5f, 0.5f);

		float DissolveValue4 = FMath::Clamp(0.5f - dissolveAnimValue, -0.5f, 0.5f);
		float DissolveValue5 = FMath::Clamp(0.5f - dissolveAnimValue, -0.5f, 0.5f);

		DynamicMaterial1->SetScalarParameterValue(TEXT("dissolve"), DissolveValue1);
		DynamicMaterial2->SetScalarParameterValue(TEXT("dissolve"), DissolveValue2);
		DynamicMaterial3->SetScalarParameterValue(TEXT("dissolve"), DissolveValue3);
		
		DynamicMaterial4->SetScalarParameterValue(TEXT("dissolve"), DissolveValue4);
		DynamicMaterial5->SetScalarParameterValue(TEXT("dissolve"), DissolveValue5);

		//UE_LOG(LogTemp, Error, TEXT("DissolveValue1: %f, DissolveValue2: %f"), DissolveValue1, DissolveValue2);
		
		if (DissolveValue1 <= -0.5f && DissolveValue2 <= -0.5f && DissolveValue3 <= -0.5f && DissolveValue4 <= -0.5f && DissolveValue5 <= -0.5f)
		{
			bItemSpawned = true;
			GetMesh()->SetVisibility(false);
			HeadStaticMesh->SetVisibility(false);
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
		//GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, "State =1");
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
	// 대기 시간이 지나면 BP_Points 중 하나를 랜덤으로 선택
	CurrentTime += DeltaTime;
	if (CurrentTime > IdleDelayTime && BP_PointsArray.Num() > 0)
	{
		/*do {
			randomIndex = FMath::RandRange(0, BP_PointsArray.Num() - 1);
		} while (BP_PointsArray[randomIndex]->GetActorLocation() == LastVisitedPoint);*/

		randomIndex = FMath::RandRange(0, BP_PointsArray.Num() - 1);
		TargetPoint = BP_PointsArray[randomIndex]->GetActorLocation();

		
		LastVisitedPoint = TargetPoint;
		UE_LOG(LogTemp, Warning, TEXT("%d"), randomIndex);
		UE_LOG(LogTemp, Warning, TEXT("TargetPoint Location: X=%f, Y=%f, Z=%f"), 
		TargetPoint.X, TargetPoint.Y, TargetPoint.Z);
		
		SetState(CleanerState::Move);
		CurrentTime = 0;
		bIsMoving = false;
	}

	if (MainCharacter->curState==EPlayerState::Talking)
	{
		SetState(CleanerState::Stop); // Stop 상태로 변경
		
		//UE_LOG(LogTemp,Warning,TEXT("%s,%s"),*CALLINFO,TEXT("TickIdle->stop"));
	}
}

void ANPC_Cleaner::TickMove(const float& DeltaTime)
{
	//AI->MoveToLocation(TargetPoint);
	//TargetPoint = BP_PointsArray[randomIndex]->GetActorLocation();
	//AI->MoveToLocation(TargetPoint);

	if (bIsMoving == false)
	{
		AI->MoveToActor(BP_PointsArray[randomIndex]);
		bIsMoving = true;
	}

	float DistanceToTarget = FVector::Dist(GetActorLocation(),TargetPoint);

	// 거리를 화면에 출력
	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Distance to Target: %f"), DistanceToTarget));
	}

	//UE_LOG(LogTemp, Warning, TEXT("TargetPoint Location: X=%f, Y=%f, Z=%f"), 
		TargetPoint.X, TargetPoint.Y, TargetPoint.Z);
	
	// 목표 지점 근처에 도달하면 Cleaning 상태로 전환
	if (DistanceToTarget <= 170.f)
	{
		SetState(CleanerState::Cleaning);
	}
	//UE_LOG(LogTemp,Warning,TEXT("%s,%s"),*CALLINFO,TEXT("TickMove"));
	if (MainCharacter->curState==EPlayerState::Talking)
	{
		SetState(CleanerState::Stop); // Stop 상태로 변경
		UE_LOG(LogTemp,Error,TEXT("%s,%s"),*CALLINFO,TEXT("TickMove->stop"));
	}
}

void ANPC_Cleaner::TickCleaning(const float& DeltaTime)
{
	if (MainCharacter->curState==EPlayerState::Talking)
	{
		SetState(CleanerState::Stop); // Stop 상태로 변경
		UE_LOG(LogTemp,Warning,TEXT("%s,%s"),*CALLINFO,TEXT("TickCleaning->stop"));
	}
	
	if (AI) 
	{
		AI->StopMovement();
	}
	//멈춰서 청소 애니메이션 진행 -> 그다음 움직임 상태로 진행
	if (!CleanerAnim->Montage_IsPlaying(CleanerAnim->CleaningMontage)) {
		CleanerAnim->playCleaningMontage();
	}
	
}

void ANPC_Cleaner::TickStop(const float& DeltaTime)
{
	//멈춤 상태로 변경, 만약 플레이어가 대화창 UI를 종료했을 시 Idle로 변경
	// 플레이어가 대화창 UI를 종료했는지 확인
	AI->StopMovement();
	if (MainCharacter->curState!=EPlayerState::Talking)
	{
		SetState(CleanerState::Idle); // Idle 상태로 변경
	}
}


void ANPC_Cleaner::SetState(CleanerState newState)
{
	mState = newState;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("State changed to: %s"), *UEnum::GetValueAsString(mState)));
	//애니메이션 상태 지정
	//CleanerAnim->animState = mState;

	if (CleanerAnim)
	{
		CleanerAnim->animState = mState;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CleanerAnim이 null입니다."));
	}
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
// void ANPC_Cleaner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
// {
// 	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
// }


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
			
			HeadStaticMesh->SetHiddenInGame(false); //머리를 보이게 함
			
			// 스크립트 출력
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "ResultEvent");
			State=3;
			TestPC->StartEndNPCDialougue(true);
			TestPC->SetNPCDialougueText(0);

			//몸 머터리얼 수정
			DynamicMaterial1 = UMaterialInstanceDynamic::Create(DissolveMaterial1, this);
			DynamicMaterial2 = UMaterialInstanceDynamic::Create(DissolveMaterial2, this);
			DynamicMaterial3 = UMaterialInstanceDynamic::Create(DissolveMaterial3, this);

			//스테틱 메시 머리 수정
			DynamicMaterial4 = UMaterialInstanceDynamic::Create(DissolveMaterial4, HeadStaticMesh);
			DynamicMaterial5 = UMaterialInstanceDynamic::Create(DissolveMaterial5, HeadStaticMesh);
			

			if (DynamicMaterial1 && DynamicMaterial2 && DynamicMaterial3 && DynamicMaterial4 && DynamicMaterial5)
			{
				GetMesh()->SetMaterial(0, DynamicMaterial1);
				GetMesh()->SetMaterial(1, DynamicMaterial2);
				GetMesh()->SetMaterial(2, DynamicMaterial3);

				HeadStaticMesh->SetMaterial(0, DynamicMaterial4);
				HeadStaticMesh->SetMaterial(1, DynamicMaterial5);
			}
			
			//디졸브
			GetWorldTimerManager().SetTimer(TimerHandle, [this]()
			{
				bisDissolve = true; //유품 스폰 뒤에 사라짐
				ChangeCleared(); //NPC 클리어
			}, 5.0f, false);
		}
	}
}

