// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Security.h"

#include "AIController.h"
#include "../Interact/ControllableLightActor.h"
#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "NPC_Cleaner.h"
#include "NPC_Security_AnimInstance.h"
#include "NPC_Youngsoo.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/Interact/ItemActor.h"
#include "Navigation/PathFollowingComponent.h"

ANPC_Security::ANPC_Security()
{
	PawnSensingComp=CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	
	SpringArmComp2 = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp2"));
	SpringArmComp2->SetupAttachment(GetMesh(),FName(TEXT("head")));

	// 카메라를 생성해서 스프링암에 붙이고싶다.
	CameraComp2 = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp2"));
	CameraComp2->SetupAttachment(SpringArmComp2);
	
	GetComponentByClass<UCapsuleComponent>()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,ECR_Ignore);

	HeadStaticMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadStaticMeshComp"));
	HeadStaticMesh->SetupAttachment(GetMesh(),FName(TEXT("LeftHandMiddle3Socket")));

	AudioComp =CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(GetCapsuleComponent());
}

void ANPC_Security::BeginPlay()
{
	Super::BeginPlay();

	// 2층 맵이 로드되기 전까지는 중력영향을 안받고 그위치에 딱 고정되있고 싶다
	SetActorHiddenInGame(true);
	
	SetActorTickEnabled(false); //최적화를 위해 tick 막아두기 
	PawnSensingComp->SetComponentTickEnabled(false);
	
	AudioComp->Stop();
	
	Anim=Cast<UNPC_Security_AnimInstance>(GetMesh()->GetAnimInstance());
	
	// 라이트가 꺼져있다 : 랜덤한 위치으로 돌아다니기
		// 라이트가 꺼지고 랜덤으로 돌아다니 던 중 , 캐릭터 마주치면 캐릭터를 chase

	// 월드에있는 라이트가 켜져있는지 꺼져있는지 여부를 어떻게 확인 ??
	// 월드에 있는 모든 라이트를 검색해서 담아주기
	// 라이트가하나라도 켜져있으면 라이트를 향해 이동
		// 켜져있는 상태에서 캐릭터 마주치면 캐릭터를 우선순위로 삼아 chase
		// 캐릭터 -> 켜져있는 라이트중 제일 가까운것
		// 자신과 라이트 거리계산
	
	EnemyAI = Cast<AAIController>(this->GetController());

	SetPatrolPoint(GetActorLocation() , PatrolPointRadius , PatrolPoint);
	
	for (TActorIterator<AControllableLightActor> It(GetWorld(), AControllableLightActor::StaticClass()); It; ++It)
	{
		//AActor* Actor = *It;
		ControllableLightArray.Add(*It);
		
	}
	PawnSensingComp->OnSeePawn.AddDynamic(this,&ANPC_Security::OnSeePawn);

	// 돌아다닐때 거리에따라 소리내기

	if (ItemHeadBP)
	{
		// AItemActor 인스턴스 생성
		ItemHead = GetWorld()->SpawnActor<AItemActor>(ItemHeadBP);
	}
	
	if (!ItemHead){return;}
	
	ItemHead->SetActorHiddenInGame(true);
	//ItemHead->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,"RightHandMiddle3Socket");
}

void ANPC_Security::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// 지금 자기상태 출력하도록 만들기
	FString myState = UEnum::GetValueAsString(SecurityState);
	DrawDebugString(GetWorld() ,GetActorLocation() , myState , nullptr , FColor::Yellow , 0 , true , 1);

	FString myTarget = Target? TEXT("target"):TEXT("target is null");
	FVector newGetActorLoc=FVector(GetActorLocation().X,GetActorLocation().Y,GetActorLocation().Z+50.f);
	
	DrawDebugString(GetWorld() ,newGetActorLoc, myTarget , nullptr , FColor::Green , 0 , true , 1);

	DrawDebugSphere(GetWorld(), GetActorLocation(), DamagedDistance, 12, FColor::Black, false, 0.1f);

	DrawDebugSphere(GetWorld(), GetActorLocation(), AttackDistance, 12, FColor::Red, false, 0.1f);
	
	if (SecurityState==ESecurityState::Stop)
	{
		MainCharacter->StopSound();
		EnemyAI->StopMovement();
		TickAllStop(DeltaSeconds);
		return;
	}
	
	// 캐릭터와의 거리도 계산  ==> 일정거리 이상 멀어지면 다시 가까운 라이트를 켜야함
	AllLightTurnOff=true;
	// ControllableLightArray 을 순회하면서 IsTurnOn =true 인걸 검색
	for (auto light : ControllableLightArray)
	{
		// 모든 불이 꺼져있으면 NearLight=nullptr
	
		if(light->IsTurnOn) // 불이켜져있는게 하나라도 있으면 
		{
			
			AllLightTurnOff=false;
			float DistToLight =GetDistanceTo(light);

			if (MinimumLightDist>=DistToLight)
			{
				MinimumLightDist=DistToLight;
				NearLight=light;
			}
		}
		// 모든 light->IsTurnOn true 이면 NearLight 을 null값으로 만들기
	}

	if (AllLightTurnOff)
	{
		NearLight=nullptr;
		MinimumLightDist=100000;
	}

	
	if (Target&& SecurityState!=ESecurityState::Attack)
	{
		// 타겟은 심장소리 나오도록
		MainCharacter->PlayHeartSound();
		SetState(ESecurityState::ChasePlayer);
	}
	else if (NearLight&&!Target)
	{
		MainCharacter->StopSound();
		SetState(ESecurityState::TurnOff);
	}
	else if (!NearLight&&!Target)
	{
		SetState(ESecurityState::Patrol);
		MainCharacter->StopSound();
	}

	if (Target&&!(PawnSensingComp->CouldSeePawn(Target,false)))
	{
		// 보일 가능성이 없으면 
		Target=nullptr;
		UE_LOG(LogTemp,Warning,TEXT("CouldSeePawn"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString(TEXT("CouldSeePawn")));
	}

	switch ( SecurityState )
	{
	case ESecurityState::Stop:	TickAllStop(DeltaSeconds);		break;
	case ESecurityState::ChasePlayer:		TickChasePlayer(DeltaSeconds);		break;
	case ESecurityState::Patrol:		TickPatrol(DeltaSeconds);		break;
	case ESecurityState::TurnOff:	TickTurnOff(DeltaSeconds);		break;
	case ESecurityState::Attack:	TickAttack(DeltaSeconds);		break;
	}
	
}

int32 ANPC_Security::GetNPCID()
{
	return NPC_ID;
}

void ANPC_Security::SetState(ESecurityState curState)
{
	
	SecurityState=curState;
	Anim->State=curState;

	// 공격상태일 때 이동을 멈추기
	if (curState==ESecurityState::Attack)
	{
		EnemyAI->StopMovement();
	}
	else if (curState==ESecurityState::Stop)
	{

		EnemyAI->StopMovement();
	}
	// 
	
}

void ANPC_Security::OnSeePawn(APawn *OtherPawn)
{
	/*if (OtherPawn)
	{
		FString message = TEXT("Saw Actor ") + OtherPawn->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);
	}*/
	auto* testCha =Cast<ATestCharacter>(OtherPawn);
	if (testCha)
	{
		Target=testCha;
		//FString message = TEXT("Saw Actor =ATestCharacter ") + OtherPawn->GetName();
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);

		// 이때만 chase를 작동시키기
		// 공격이 아닐때
		if (SecurityState!=ESecurityState::Attack)
		{
			SetState(ESecurityState::ChasePlayer);
		}
	}
}

void ANPC_Security::TickChasePlayer(const float& DeltaTime)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "TickChasePlayer");
	
	if (EnemyAI&&Target)
	{
		EnemyAI->MoveToLocation(Target->GetActorLocation());
		GetCharacterMovement()->MaxWalkSpeed=ChaseSpeed;
		// 타겟과 나(경비원) 의 거리 계산
		FVector dir = Target->GetActorLocation() - this->GetActorLocation();
		float dist = dir.Size();

		// 가까울때 공격상태로 전이 
		if ( dist < AttackDistance )
		{
			// 공격상태로 전이하고싶다.
			SetState(ESecurityState::Attack);
		}
		
	}
	else
	{
		SetState(ESecurityState::Patrol);
	}
}

void ANPC_Security::TickPatrol(const float& DeltaTime)
{
	// 목적지를 향해서 이동하고싶다.
	//FVector dir = Target->GetActorLocation() - GetActorLocation();
	//float dist = dir.Size();
	//Me->AddMovementInput(dir.GetSafeNormal());
	GetCharacterMovement()->MaxWalkSpeed=PatrolSpeed;
	FVector destinataion = PatrolPoint;

	auto* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(destinataion);
	MoveRequest.SetAcceptanceRadius(50);

	FPathFindingQuery Query;
	EnemyAI->BuildPathfindingQuery(MoveRequest , Query);
	FPathFindingResult r = ns->FindPathSync(Query);
	// 만약 목적지가 길 위에있다면
	if (r.Result == ENavigationQueryResult::Success)
	{
		// 목적지를 향해서 이동하고싶다.
		EnemyAI->MoveToLocation(destinataion);

		EPathFollowingRequestResult::Type result = EnemyAI->MoveToLocation(PatrolPoint);
		// 만약 도착했다면 다시 랜덤한 위치를 정하고싶다.
		if ( result == EPathFollowingRequestResult::AlreadyAtGoal ||
			result == EPathFollowingRequestResult::Failed )
		{
			SetPatrolPoint(GetActorLocation() , PatrolPointRadius , PatrolPoint);
		}
		
	}
	else 	// 만약 목적지가 길 위에 없으면 
	{
		EPathFollowingRequestResult::Type result = EnemyAI->MoveToLocation(PatrolPoint);
		// 만약 도착했다면 다시 랜덤한 위치를 정하고싶다.
		if ( result == EPathFollowingRequestResult::AlreadyAtGoal ||
			result == EPathFollowingRequestResult::Failed )
		{
			SetPatrolPoint(GetActorLocation() , PatrolPointRadius , PatrolPoint);
		}
	}
	
}

void ANPC_Security::TickTurnOff(const float& DeltaTime)
{

	if (EnemyAI&&NearLight)
	{
		GetCharacterMovement()->MaxWalkSpeed=TurnOffSpeed;
		// 여기서 알아서 장애물 회피해서 이동해야함 
		EnemyAI->MoveToLocation(NearLight->SphereComp->GetComponentLocation(),0);
	}
}

void ANPC_Security::TickAttack(const float& DeltaTime)
{
	
}

void ANPC_Security::TickAllStop(const float& DeltaTime)
{
	EnemyAI->StopMovement();
}

bool ANPC_Security::SetPatrolPoint(FVector origin, float radius, FVector& dest)
{
	// 길위의 랜덤한 위치를 정하고싶다.
	auto* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool isSuccess = ns->GetRandomReachablePointInRadius(origin, radius, loc);
	// 만약 성공했다면
	if ( isSuccess )
	{
		// 그 위치를 dest에 적용하고싶다.
		dest = loc.Location;
	}
	return isSuccess;
}

void ANPC_Security::OnMyAttackStart()
{
	Anim->bAttack = false;
	// 주인공캐릭터 멈추게 (이동못하게하고) 피격당하는 애니 실행되도록
	// MainCharacter->PlayDamagedAnimMontage();
}

void ANPC_Security::OnMyAttackMiddle()
{
	if (!Target){return;}
	float dist = GetDistanceTo(Target);
	if ( dist <= AttackDistance) {
		// 플레이어에게 데미지를 입히고싶다.
		MainCharacter->DamagedToSecurity();
	}
	else
	{
		// 이동상태로 전이하고싶다.==> 이게 어색함 .앞에 캐릭터있는데도 다른곳으로 갈때 있음 시야각에 있음 무조건 캐릭터 다시 채이스하도록
		Target=MainCharacter;
		SetState(ESecurityState::ChasePlayer);
	}
}

void ANPC_Security::OnMyAttackEnd()
{
	// 거리체크
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(TEXT("OnMyAttackEnd")));
	if (!Target){return;}
	float dist = GetDistanceTo(Target);
	
	if (dist <= AttackDistance)
	{
		Target=MainCharacter;
		SetState(ESecurityState::Attack);
	}
	// 그렇지 않다면 
	else {
		// 이동상태로 전이하고싶다.
		Target=MainCharacter;
		SetState(ESecurityState::ChasePlayer);
	}
}

void ANPC_Security::ChangeCleared()
{
	Super::ChangeCleared();
}

void ANPC_Security::EndEvent()
{
	AudioComp->Stop();
	TestPC->curNPC=this;
	
	PawnSensingComp->bEnableSensingUpdates=false;
	PawnSensingComp->bSeePawns=false;
	Target=nullptr;
	
	GetCharacterMovement()->DisableMovement();
	EnemyAI->StopMovement();
	SetState(ESecurityState::Stop);

	Anim->playIdleMontage();
	
	TestPC->SetViewTargetWithBlend(this,0.f);

	
	
	// 사라지는 효과

	FTimerHandle dissolveTimer;
	GetWorldTimerManager().SetTimer(dissolveTimer,[this]()
	{
		// 사라지는 효과
		FString color = "Yellow";
		DissolveEvent(color);
		HeadStaticMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		HeadStaticMesh->SetEnableGravity(true);
		HeadStaticMesh->SetSimulatePhysics(true);
		//HeadStaticMesh->SetVisibility(false);
		
		//ItemHead->SetActorLocation(HeadStaticMesh->GetComponentLocation());
		//ItemHead->SetActorRotation(HeadStaticMesh->GetComponentRotation());
	},1.0f,false);

	//영수 캐스팅 후 보이게 하기
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Youngsoo")), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		ANPC_Youngsoo* Youngsoo = Cast<ANPC_Youngsoo>(Actor);
		if (Youngsoo)
		{
			UE_LOG(LogTemp,Warning,TEXT("Youngsoo"));
			//영수 보이게 하기
			Youngsoo->GetMesh()->SetHiddenInGame(false);
			Youngsoo->GetComponentByClass<UCapsuleComponent>()->SetCollisionProfileName(FName("NPC"));
			Youngsoo->Scarf->SetHiddenInGame(false);
		}
	}
	

	ChangeCleared(); // 더이상 상호작용 안하도록 막고
	
	FTimerHandle CameraViewChangePlayerTimer;
	GetWorldTimerManager().SetTimer(CameraViewChangePlayerTimer,[this]()
	{
		TestPC->CameraViewChangePlayer();
		
	},8.0f,false);

	FTimerHandle headChangeTimer;
	GetWorldTimerManager().SetTimer(headChangeTimer , [this]()
	{
		HeadStaticMesh->SetHiddenInGame(true);
		ItemHead->SetActorHiddenInGame(false);

		float LOCZ=HeadStaticMesh->GetComponentLocation().Z+5;
		FVector newLOC=FVector(HeadStaticMesh->GetComponentLocation().X,HeadStaticMesh->GetComponentLocation().Y,LOCZ);
		
		ItemHead->SetActorLocation(newLOC);
		
		ItemHead->SetActorRotation(HeadStaticMesh->GetComponentRotation());
	} , 9.5f , false);

	// 그 자리에 아이템 머리 스폰하기 
}
