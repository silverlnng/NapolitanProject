// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Security.h"

#include "AIController.h"
#include "ControllableLightActor.h"
#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "NapolitanProject/YJ/TestCharacter.h"
#include "Navigation/PathFollowingComponent.h"

ANPC_Security::ANPC_Security()
{
	PawnSensingComp=CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
}

void ANPC_Security::BeginPlay()
{
	Super::BeginPlay();

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
	
}

void ANPC_Security::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	

	// Target을 항상 탐색
	//ATestCharacter 를 탐색 근데 
	//PawnSensingComp->OnSeePawn.Broadcast(MainCha);
	// ANPC_Security::OnSeePawn 는 계속 작동 

	/*if (MainCha)
	{
		FString message = TEXT("Saw Actor ") + MainCha->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);
	}*/

	
	// 캐릭터와의 거리도 계산  ==> 일정거리 이상 멀어지면 다시 가까운 라이트를 켜야함
	if (!(PawnSensingComp->CouldSeePawn(Target,true)))
	{
		Target=nullptr;
	}

	// 지금 자기상태 출력하도록 만들기
	FString myState = UEnum::GetValueAsString(SecurityState);
	DrawDebugString(GetWorld() , GetOwner()->GetActorLocation() , myState , nullptr , FColor::Yellow , 0 , true , 1);

	
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

	if (Target)
	{
		SetState(ESecurityState::ChasePlayer);
	}
	else if (NearLight&&!Target)
	{
		SetState(ESecurityState::TurnOff);
	}
	else if (!NearLight&&!Target)
	{
		SetState(ESecurityState::Patrol);
	}

	switch ( SecurityState )
	{
	case ESecurityState::ChasePlayer:		TickChasePlayer(DeltaSeconds);		break;
	case ESecurityState::Patrol:		TickPatrol(DeltaSeconds);		break;
	case ESecurityState::TurnOff:	TickTurnOff(DeltaSeconds);		break;
	}
	
}

void ANPC_Security::SetState(ESecurityState curState)
{
	SecurityState=curState;
	
}

void ANPC_Security::OnSeePawn(APawn *OtherPawn)
{
	if (OtherPawn)
	{
		FString message = TEXT("Saw Actor ") + OtherPawn->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);
	}
	auto* testCha =Cast<ATestCharacter>(OtherPawn);
	if (testCha)
	{
		Target=testCha;
		FString message = TEXT("Saw Actor =ATestCharacter ") + OtherPawn->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);

		// 이때만 chase를 작동시키기
		SetState(ESecurityState::ChasePlayer);
	}
}

void ANPC_Security::TickChasePlayer(const float& DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "TickChasePlayer");
	if (EnemyAI&&Target)
	{
		EnemyAI->MoveToLocation(Target->GetActorLocation());
	}
}

void ANPC_Security::TickPatrol(const float& DeltaTime)
{
	// 목적지를 향해서 이동하고싶다.
	//FVector dir = Target->GetActorLocation() - GetActorLocation();
	//float dist = dir.Size();
	//Me->AddMovementInput(dir.GetSafeNormal());

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
		// 만약 목적지와의 거리가 공격 가능거리라면
		EPathFollowingRequestResult::Type result = EnemyAI->MoveToLocation(PatrolPoint);
		// 만약 도착했다면 다시 랜덤한 위치를 정하고싶다.
		if ( result == EPathFollowingRequestResult::AlreadyAtGoal ||
			result == EPathFollowingRequestResult::Failed )
		{
			SetPatrolPoint(GetActorLocation() , PatrolPointRadius , PatrolPoint);
		}
	}
	/*else
	{
		EPathFollowingRequestResult::Type result = EnemyAI->MoveToLocation(PatrolPoint);
		// 만약 도착했다면 다시 랜덤한 위치를 정하고싶다.
		if ( result == EPathFollowingRequestResult::AlreadyAtGoal ||
			result == EPathFollowingRequestResult::Failed )
		{
			SetPatrolPoint(GetActorLocation() , PatrolPointRadius , PatrolPoint);
		}
	}*/
}

void ANPC_Security::TickTurnOff(const float& DeltaTime)
{

	if (EnemyAI&&NearLight)
	{
		EnemyAI->MoveToLocation(NearLight->GetActorLocation(),0);
		UE_LOG(LogTemp, Warning, TEXT("NearLight: %s"), *NearLight->GetActorLocation().ToString());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "TickTurnOff");
	}
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
