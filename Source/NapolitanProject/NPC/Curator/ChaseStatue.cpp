// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaseStatue.h"
#include "Components/CapsuleComponent.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include <NavigationSystem.h>

#include "EngineUtils.h"
#include "NPC_CuratorAnim.h"
#include "YSEvanceUI.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/NapolitanProject.h"
#include "NapolitanProject/ArtMap/SunFloorDoorToLobby.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/YJ/DeadEndingWidget.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/SaveGISubsystem.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/Interact/ItemActor.h"
#include "NapolitanProject/Interact/SouvenirActor.h"
#include "NapolitanProject/LevelEvent/LightControlActor.h"
#include "Navigation/PathFollowingComponent.h"


// Sets default values
AChaseStatue::AChaseStatue()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));

	if (TempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
	}

	CSCol = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CSCol"));
	CSCol->SetupAttachment(GetMesh());
	CSCol->SetCapsuleHalfHeight(90.f);
	CSCol->SetCapsuleRadius(90.f);

	// 몬스터 카메라 생성 및 초기화
	MonsterCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MonsterCamera"));
	MonsterCamera->SetupAttachment(GetMesh(),FName(TEXT("HeadSocket"))); // 루트에 부착
	MonsterCamera->bUsePawnControlRotation = false; // 플레이어 조작 방지

}

// Called when the game starts or when spawned
void AChaseStatue::BeginPlay()
{
	Super::BeginPlay();

	me = this;

	mState = ChaseStatueState::Idle;

	ChaseAI = Cast<AAIController>(me->GetController());
	if (!ChaseAI)
	{
		UE_LOG(LogTemp, Error, TEXT("ChaseAI is null! Make sure the AIController is correctly set."));
		return;
	}

	CSCol->OnComponentBeginOverlap.AddDynamic(this, &AChaseStatue::CuratorOverlap);

	//캐릭터 애니메이션
	CuratorAnim = Cast<UNPC_CuratorAnim>(GetMesh()->GetAnimInstance());

	bClear = false;
	bItemSpawned = false;

	DoorToLobby = Cast<ASunFloorDoorToLobby>(UGameplayStatics::GetActorOfClass(GetWorld(), ASunFloorDoorToLobby::StaticClass()));

	for (TActorIterator<ALightControlActor> It(GetWorld(), ALightControlActor::StaticClass()); It; ++It)
	{
		LightControlActor = *It;
	}

	bIsDeadEnding = false;
}

// Called every frame
void AChaseStatue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("%s %s"), *UEnum::GetValueAsString(MainCharacter->curState), bClear ? TEXT("true") : TEXT("false"));
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *UEnum::GetValueAsString(mState));

	// 지금 자기상태 출력하도록 만들기
	//FString myState = UEnum::GetValueAsString(mState);
	//DrawDebugString(GetWorld() ,GetActorLocation() , myState , nullptr , FColor::Yellow , 0 , true , 1);
	switch (mState)
	{
	case ChaseStatueState::Idle:
		TickIdle(DeltaTime);
		break;
	case ChaseStatueState::Move:
		TickMove(DeltaTime);
		break;
	}
}

// Called to bind functionality to input
void AChaseStatue::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AChaseStatue::TickIdle(const float& DeltaTime)
{
	//이 상태일때는 무조건 대기, 플레이어가 아이템을 주었을때부터 추격을 시작함.
	//플레이어가 안에 들어왔고, 그 안에서 자신을 바라보지 않을 때 우선은 대기 상태로 있는다.
	FVector targetLoc = MainCharacter->GetActorLocation();
	FVector myLoc = me->GetActorLocation();
	FVector dirR = targetLoc - myLoc;
	FRotator rot = dirR.Rotation();
	
	me->SetActorRotation(FRotator(0, rot.Yaw, 0));

	//만약 플레이어가 대화를 끝냈을때 움직임을 선택
	if(MainCharacter->curState == EPlayerState::Idle && bClear == true)
	{
		//큐레이터 맵의 사운드가 변경
		
		//Move로 변경
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [this]()
		{
			//UE_LOG(LogTemp, Error, TEXT("모드 변경"));
			SetState(ChaseStatueState::Move); //몇초 뒤 움직임으로 변경
		}, 4.0f, false);
	}
}

void AChaseStatue::TickMove(const float& DeltaTime)
{
	
	FVector targetLoc = MainCharacter->GetActorLocation();
	FVector myLoc = me->GetActorLocation();
	FVector dirR = targetLoc - myLoc;
	FRotator rot = dirR.Rotation();
	
	me->SetActorRotation(FRotator(0, rot.Yaw, 0));
	//AddMovementInput(dirR.GetSafeNormal());
	//UE_LOG()

	//NavigationSystem 객체 얻어오기
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	//목적지 길 찾기 경로 데이터 검색
	FPathFindingQuery query;
	FAIMoveRequest req;

	//목적지에서 인지할 수 잇는 범위
	req.SetAcceptanceRadius(5000);
	req.SetGoalLocation(targetLoc);

	//길찾기를 위한 쿼리 생성
	ChaseAI->BuildPathfindingQuery(req, query);
	//길찾기 결과 가져오기
	FPathFindingResult r = ns->FindPathSync(query);

	// 만약 목적지가 길 위에있다면
	if (r.Result == ENavigationQueryResult::Success)
	{
		//타깃쪽으로 이동
		ChaseAI->MoveToLocation(targetLoc);
	}

	//만약 큐레이터와의 거리가 200일 경우 사망
	float Distance = FVector::Dist(targetLoc, myLoc);
	if(Distance < 200.0f && bIsDeadEnding == false)
	{
		CuratorDead();
		bIsDeadEnding = true; //한번만 호출되도록
	}
}


//랜덤한 위치를 가져오기
bool AChaseStatue::GetRandomPositionNavMesh(FVector CenterLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(CenterLocation, radius, loc);
	dest = loc.Location;
	return result;
}

//큐레이터와 부딪혔을때 이벤트 -> 현재는 간단하게 데드엔딩으로 보임
void AChaseStatue::CuratorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		auto* cha=Cast<ATestCharacter>(OtherActor);
		//플레이어 + 움직이는 상태일때
		if (cha && mState == ChaseStatueState::Move)
		{
			// 끝나는 엔딩 위젯 나오도록 하기
			if (PlayerHUD && PlayerHUD->DeadEndingWidgetUI)
			{
				PlayerHUD->DeadEndingWidgetUI->SetVisibility(ESlateVisibility::Visible);
				
				//대기 상태로 변경 => 여기는 수정 필요
				SetState(ChaseStatueState::Idle); 
			}
		}
		
	}
}

void AChaseStatue::ResultEvent(int32 result)
{
	//이벤트 발생
	if(1==State)
	{
		//노인이 지닌 아이템을 소유하고 있을때
		if(0==result)
		{
			bItemSpawned = true;
			
			SpawnItems(); //유품 스폰 및 회수
			
			State=2; // 다음 state으로 넘어간다음
			TestPC->StartEndNPCDialougue(true);
			TestPC->SetNPCDialougueText(0); // npc 대본에서 다시 시작
			
		}
	}
	else if(2==State)
	{
		if(0==result)
		{
			State=3; // 다음 state으로 넘어간다음
			TestPC->StartEndNPCDialougue(true);
			TestPC->SetNPCDialougueText(0); // npc 대본에서 다시 시작 
		}
	}
	else if(3==State)
	{
		//노인이 지닌 아이템을 소유하고 있을때
		if(0==result)
		{
			//큐레이터 맵 사운드 변경

			bClear = true; //움직임 모드로 변경
			if(DoorToLobby)
			{
				DoorToLobby->bOneMove = true; //이제 큐레이터 방 나갈 수 있음
				//UE_LOG(LogTemp, Warning, TEXT("방 나갈 수 있음"));
			}

			//우선 클리어 선언
			IsCleared=true;
			//GetComponentByClass<UCapsuleComponent>()->SetCollisionProfileName(FName("ClearedNPC"));
			SaveGI->ClearedNPC.Add(GetNPCID());

			State=4; // 다음 state으로 넘어간다음
			TestPC->StartEndNPCDialougue(true);
			TestPC->SetNPCDialougueText(0); // npc 대본에서 다시 시작
			
		}
	}
	else if(4==State)
	{
		if(0==result)
		{
			//추격전 시작

			FTimerHandle CuraTimer;
			GetWorld()->GetTimerManager().SetTimer(CuraTimer,[this]()
			{
				TestPC->StartEndNPCDialougue(false); //결과 대화창 출력
				SetState(ChaseStatueState::Move);
			},8.0f,false);
		}
	}
	
}

void AChaseStatue::Interact()
{
	Super::Interact();
}

int32 AChaseStatue::GetNPCID()
{
	return NPC_ID;
}

int32 AChaseStatue::GetState()
{
	return State;
}

void AChaseStatue::ChangeCleared()
{
	Super::ChangeCleared();
}

void AChaseStatue::SetState(ChaseStatueState newstate)
{
	mState = newstate;
	if(CuratorAnim)
	{
		CuratorAnim->animState = mState;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CuratorAnim이 null입니다."));
	}
}

void AChaseStatue::SpawnItems()
{
	//여기서는 유품 아이템이 자동으로 인벤토리에 들어오도록 해야함(추격이 있기 때문에)
	//아이템 스폰
	if(bItemSpawned)
	{
		// 발끝 위치를 기준으로 스폰 위치 설정
		FVector FootLocation = GetMesh()->GetSocketLocation(FName("ItemSpawn"));
		FTransform SpawnTransform(FootLocation);

		// 블루프린트에서 설정된 SouvenirClass로 스폰, 청소부는 출력하는 아이템이 없음
		SouvenirActor = GetWorld()->SpawnActor<ASouvenirActor>(SouvenirClass, SpawnTransform );
		if (SouvenirActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("큐레이터 유품 회수"));
			SouvenirActor->Tags.Add(FName("Souvenir"));
			SouvenirActor->OnPickup(); //유품 회수
			
		}
	}

	bItemSpawned = false; //한번만 스폰되도록
}

void AChaseStatue::LightEffect()
{
	if (LightControlActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("LightControlActor")));
		LightControlActor->StartSineFlicker(0,1,1.5f,25.f,4408.f);
	}
}

void AChaseStatue::CuratorDead()
{
	//카메라 변경
	//SwitchToMonsterCamera();

	//애니메이션 실행

	//데드 엔딩 UI 뜨기
	FTimerHandle UITimer2;
	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this]()
	{
		if (PlayerHUD)
		{
		
			PlayerHUD->PlayDeadVignetteEffect();
			UE_LOG(LogTemp, Warning, TEXT("CreateDieUI1"));
		}
	},2.5f,false);
	
	//시간 지연 주고 사망 UI 나오도록
	FTimerHandle UITimer;
	GetWorldTimerManager().SetTimer(UITimer, [this]()
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateDieUI2"));
		MainCharacter->SetPlayerState(EPlayerState::UI);

		if(PlayerHUD && PlayerHUD->DeadEndingWidgetUI)
		{
			PlayerHUD->DeadEndingWidgetUI->SetVisibility(ESlateVisibility::Visible);
			PlayerHUD->DeadEndingWidgetUI->SetTextBlock_description(description);
		}
	},3.5f,false); //사망
}

void AChaseStatue::SwitchToMonsterCamera()
{
	if(TestPC && MonsterCamera)
	{
		// 1. 먼저 이 액터를 ViewTarget으로 설정
		TestPC->SetViewTargetWithBlend(this, 0.01f);
            
		// 2. 그 다음 액터의 활성 카메라를 MonsterCamera로 변경
		SetActorViewTarget(MonsterCamera);

		//3. 플레이어 메쉬 숨기기
		MainCharacter->GetMesh()->SetHiddenInGame(true);

		// 카메라 흔들기 실행
		if (TestPC->PlayerCameraManager)
		{
			if (DeathCameraShakeClass)
			{
				TestPC->PlayerCameraManager->StartCameraShake(DeathCameraShakeClass);
				
			}
		}
	}
}

void AChaseStatue::SetActorViewTarget(UCameraComponent* TargetCamera)
{
	if (TargetCamera)
	{
		// 다른 카메라들 비활성화
		TArray<UCameraComponent*> CameraComponents;
		GetComponents<UCameraComponent>(CameraComponents);
        
		for (UCameraComponent* Camera : CameraComponents)
		{
			Camera->SetActive(false);
		}
        
		// MonsterCamera만 활성화
		TargetCamera->SetActive(true);
	}
}



