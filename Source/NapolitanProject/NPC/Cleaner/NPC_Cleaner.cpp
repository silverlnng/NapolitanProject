// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Cleaner.h"

#include "AIController.h"
#include "NPCCleanerAnim.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/SaveGISubsystem.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"


// Sets default values
ANPC_Cleaner::ANPC_Cleaner()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HeadStaticMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadStaticMeshComp"));
	HeadStaticMesh->SetupAttachment(GetMesh(),"HeadSocket");

	// 머티리얼 경로를 설정 (경로는 프로젝트에 맞게 수정)
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder1(TEXT("/Script/Engine.Material'/Game/Bada/Effect/Cleaner/Dissolve_Cleaner_Body.Dissolve_Cleaner_Body'"));
	if (MaterialFinder1.Succeeded())
	{
		DissolveMaterial1 = MaterialFinder1.Object;
		DynamicMaterial1 = UMaterialInstanceDynamic::Create(DissolveMaterial1, this);
	}
   
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder2(TEXT("/Script/Engine.Material'/Game/Bada/Effect/Cleaner/Dissolve_Cleaner_Head.Dissolve_Cleaner_Head'"));
	if (MaterialFinder2.Succeeded())
	{
		DissolveMaterial2 = MaterialFinder2.Object;
		DynamicMaterial2 = UMaterialInstanceDynamic::Create(DissolveMaterial2, this);
	}

}

// Called when the game starts or when spawned
void ANPC_Cleaner::BeginPlay()
{
	Super::BeginPlay();

	AI = Cast<AAIController>(GetController());
	CleanerAnim = Cast<UNPCCleanerAnim>(GetMesh()->GetAnimInstance());
	HeadStaticMesh->SetHiddenInGame(true);

	bIsMoving = false;
	bCleaning = false;

	// CleanerPoint 태그를 가진 액터들을 찾아서 배열에 저장
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("CleanerPoint"), CleanerPointActors);
	
	// 첫 번째 타겟 포인트를 랜덤으로 설정 (첫 시작이므로 이전 방문 지점이 없음)
	if (CleanerPointActors.Num() > 0)
	{
		randomIndex = FMath::RandRange(0, CleanerPointActors.Num() - 1);
		LastVisitedPointActor = CleanerPointActors[randomIndex];
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
}

// Called every frame
void ANPC_Cleaner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MainCharacter)
	{
		return;
	}

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

	//랜덤 시간이 지날 경우 어떤 경우든 청소모드로 변경
	CleaningTime += DeltaTime;
	//40초에서 2분 랜덤 시간 설정
	RandomCleaningTime = FMath::RandRange(20.0f, 100.0f);
	// 40~120초 사이 시간이 지나면 Cleaning 상태로 전환
	if (CleaningTime >= RandomCleaningTime)
	{
		bCleaning = true;
		CleaningTime = 0;
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
	if (SaveGI->ClearedNPC.Contains(4))
	{
		State =3;
	}
	
	return State;
}

void ANPC_Cleaner::ChangeCleared()
{
	Super::ChangeCleared();
}

void ANPC_Cleaner::TickIdle(const float& DeltaTime)
{
	// 널 포인터 체크 추가
	if (!MainCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("MainCharacter is nullptr in TickIdle!"));
		return;
	}
	
	// 대기 시간 계산
	CurrentTime += DeltaTime;
	if (CurrentTime > IdleDelayTime)
	{
		// CleanerPoint 액터들이 존재하는지 확인
		if (CleanerPointActors.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("No CleanerPoint actors found!"));
			return;
		}

		// 첫 시작시에만 목표 지점 설정 (이후로는 TickMove에서 연속 처리)
		if (!TargetPointActor)
		{
			// 임시 배열을 만들어서 마지막 방문한 액터를 제외
			TArray<AActor*> AvailablePoints = CleanerPointActors;
			if (LastVisitedPointActor && AvailablePoints.Contains(LastVisitedPointActor))
			{
				AvailablePoints.Remove(LastVisitedPointActor);
			}

			// 사용 가능한 포인트가 있는지 확인
			if (AvailablePoints.Num() > 0)
			{
				// 랜덤으로 다음 목표 지점 선택
				randomIndex = FMath::RandRange(0, AvailablePoints.Num() - 1);
				TargetPointActor = AvailablePoints[randomIndex];
				TargetPoint = TargetPointActor->GetActorLocation();
				LastVisitedPointActor = TargetPointActor;
			}
		}

		// Move 상태로 전환
		SetState(CleanerState::Move);
		CurrentTime = 0; // 시간 초기화
		bIsMoving = true;
	}

	//Tick에서 bCleaning이 true가 되었을 경우 어떤 상태이든 청소 상태로 돌입
	if(bCleaning)
	{
		SetState(CleanerState::Cleaning);
		bCleaning = false;
	}

	if (MainCharacter->curState==EPlayerState::Talking)
	{
		SetState(CleanerState::Stop); // Stop 상태로 변경
	}
}

void ANPC_Cleaner::TickMove(const float& DeltaTime)
{
	if (AI && bIsMoving)
	{
		// 목표 지점으로 이동
		AI->MoveToLocation(TargetPoint);
		// 목표 지점 근처에 도달하면 바로 새로운 목적지 설정
		if (FVector::Dist(GetActorLocation(), TargetPoint) <= 100.f)
		{
			// CleanerPoint 액터들이 존재하는지 확인
			if (CleanerPointActors.Num() == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("No CleanerPoint actors found in TickMove!"));
				SetState(CleanerState::Idle);
				return;
			}

			// 임시 배열을 만들어서 마지막 방문한 액터를 제외
			TArray<AActor*> AvailablePoints = CleanerPointActors;
			if (LastVisitedPointActor && AvailablePoints.Contains(LastVisitedPointActor))
			{
				AvailablePoints.Remove(LastVisitedPointActor);
			}

			// 사용 가능한 포인트가 있는지 확인
			if (AvailablePoints.Num() > 0)
			{
				// 랜덤으로 다음 목표 지점 선택
				randomIndex = FMath::RandRange(0, AvailablePoints.Num() - 1);
				TargetPointActor = AvailablePoints[randomIndex];
				TargetPoint = TargetPointActor->GetActorLocation();
				LastVisitedPointActor = TargetPointActor;

				// 바로 새로운 목적지로 이동 (상태 변경 없이 계속 Move 상태 유지)
				AI->MoveToLocation(TargetPoint);
			}
			else
			{
				// 모든 포인트를 방문했다면 마지막 방문 지점 리셋하고 새로운 목적지 설정
				LastVisitedPointActor = nullptr;
				if (CleanerPointActors.Num() > 0)
				{
					randomIndex = FMath::RandRange(0, CleanerPointActors.Num() - 1);
					TargetPointActor = CleanerPointActors[randomIndex];
					TargetPoint = TargetPointActor->GetActorLocation();
					LastVisitedPointActor = TargetPointActor;
					AI->MoveToLocation(TargetPoint);
				}
			}
		}
	}

	//Tick에서 bCleaning이 true가 되었을 경우 어떤 상태이든 청소 상태로 돌입
	if(bCleaning)
	{
		SetState(CleanerState::Cleaning);
		bCleaning = false;
	}

	if (MainCharacter->curState==EPlayerState::Talking)
	{
		SetState(CleanerState::Stop); // Stop 상태로 변경
	}
}

void ANPC_Cleaner::TickCleaning(const float& DeltaTime)
{
	if (MainCharacter->curState==EPlayerState::Talking)
	{
		SetState(CleanerState::Stop); // Stop 상태로 변경
		//UE_LOG(LogTemp,Warning,TEXT("%s,%s"),*CALLINFO,TEXT("TickCleaning->stop"));
	}
	//MobPointActor->SetActorHiddenInGame(false);
	
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
	if (MainCharacter->curState!=EPlayerState::Talking && bIsResult == false)
	{
		SetState(CleanerState::Idle); // Idle 상태로 변경
	}
}


void ANPC_Cleaner::SetState(CleanerState newState)
{
	mState = newState;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("State changed to: %s"), *UEnum::GetValueAsString(mState)));
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

void ANPC_Cleaner::SpawnFootsDecal()
{
	if(!FootstepDecalMaterial) return;

	//UE_LOG(LogTemp, Warning, TEXT("SpawnFootsDecal"));

	//소켓 위치 가져오기
	FVector FootLocation = GetMesh()->GetSocketLocation(FName("BloodSpawn"));
	FVector TraceStart = FootLocation;
	FVector TraceEnd = TraceStart - FVector(0, 0, 100.f); //발 밑 방향으로 레이캐스트

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); //자기 자신은 무시

	//바닥 충돌 검사
	if(GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		//FRotator DecalRotation = HitResult.Normal.Rotation();
		FRotator DecalRotation = FRotator(-90.f, 0.f, 0.f); // Pitch = -90
		UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),
			FootstepDecalMaterial,
			DecalSize,
			HitResult.Location,
			DecalRotation,  // 바닥 방향에 맞춘 회전
			DecalLifetime
		);

		//디버그 라인
		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.f, 0, 1.f);
		//DrawDebugSphere(GetWorld(), HitResult.Location, 10.f, 12, FColor::Green, false, 1.f);
		
	}
}


void ANPC_Cleaner::ResultEvent(int32 result)
{
	//이거 자체가 일단 호출되면 캐릭터의 움직임을 멈춤 상태로
	SetState(CleanerState::Stop);
	
	if (3==State)
	{
		if (0==result)
		{

			if (MissionClearSoundWave)
			{
				UGameplayStatics::PlaySound2D(this, MissionClearSoundWave);
			}
			
			if (MainCharacter->curItem)
			{
				MainCharacter->curItem->PutDown();
			}

			if (AI) 
			{
				AI->StopMovement(); //멈춤
			}

			bIsResult = true;
			
			HeadStaticMesh->SetHiddenInGame(false); //머리를 보이게 함

			// 이제 사용한 머리는 SavedItems 에서 제거해야함
			if (ItemHead && SaveGI->SavedItems.Contains(ItemHead->GetClass()))
			{
				SaveGI->SavedItems.Remove(ItemHead->GetClass());
			}

			// 인벤토리에서 버튼도 비활성화 하기
			ItemHead->Remove();
			
			// 스크립트 출력
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "ResultEvent");
			//State=3;
			//TestPC->StartEndNPCDialougue(true);
			//TestPC->SetNPCDialougueText(0);
			//TestPC->CallCurNPCResultEvent(0);
			
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