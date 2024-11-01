// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Security.h"

#include "ControllableLightActor.h"
#include "EngineUtils.h"
#include "NapolitanProject/YJ/TestCharacter.h"

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

	
	
	for (TActorIterator<AControllableLightActor> It(GetWorld(), AControllableLightActor::StaticClass()); It; ++It)
	{
		//AActor* Actor = *It;
		ControllableLightArray.Add(*It);
		
	}
	//ControllableLightArray.Add()
	// 켜져있고 나와의 거리를 계산
	
	// 돌아다닐때 거리에따라 소리

	PawnSensingComp->OnSeePawn.AddDynamic(this,&ANPC_Security::OnSeePawn);
	
	// AddDynamic  하고 실행은 어디서 ???
}

void ANPC_Security::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	// ControllableLightArray 을 순회하면서 IsTurnOn =true 인걸 검색

	// Target을 항상 탐색
	//ATestCharacter 를 탐색 근데 
	PawnSensingComp->OnSeePawn.Broadcast(MainCha);
	// ANPC_Security::OnSeePawn 는 계속 작동 

	/*if (MainCha)
	{
		FString message = TEXT("Saw Actor ") + MainCha->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);
	}*/

	
	// 캐릭터와의 거리도 계산  ==> 일정거리 이상 멀어지면 다시 가까운 라이트를 켜야함
	//float DistToCharacter =GetDistanceTo(MainCharacter);
	//float DistToLight

	
	for (auto light : ControllableLightArray)
	{
		if (!light->IsTurnOn)
		{
			// 꺼져있는걸로 거리 계산
			float DistToLight =GetDistanceTo(light);

			if (MinimumLightDist<=DistToLight)
			{
				MinimumLightDist=DistToLight;
				NearLight=light;
			}
			
		}
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
		FString message = TEXT("Saw Actor =ATestCharacter ") + OtherPawn->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);
	}
	
	

	// TODO: game-specific logic
}