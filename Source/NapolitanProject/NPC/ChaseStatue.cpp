// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaseStatue.h"
#include "Components/CapsuleComponent.h"
#include "NapolitanProject/YJ/TestCharacter.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include <NavigationSystem.h>

#include "YSEvanceUI.h"
#include "NapolitanProject/YJ/DeadEndingWidget.h"
#include "NapolitanProject/YJ/PlayerHUD.h"
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
	CSCol->SetCapsuleHalfHeight(90.f);
	CSCol->SetCapsuleRadius(90.f);

}

// Called when the game starts or when spawned
void AChaseStatue::BeginPlay()
{
	Super::BeginPlay();
	
	me = this;

	ChaseAI = Cast<AAIController>(me->GetController());

	CSCol->OnComponentBeginOverlap.AddDynamic(this, &AChaseStatue::CuratorOverlap);
	
}

// Called every frame
void AChaseStatue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (mState)
	{
	case ChaseStatueState::Patrol:
		TickPatrol(DeltaTime);
	case ChaseStatueState::Move:
		TickMove(DeltaTime);
	case ChaseStatueState::Attack:
		TickAttack(DeltaTime);
	}
}

// Called to bind functionality to input
void AChaseStatue::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AChaseStatue::TickPatrol(const float& DeltaTime)
{
	
}

void AChaseStatue::TickMove(const float& DeltaTime)
{
	//플레이어가 안에 들어왔고, 그 안에서 자신을 바라보지 않을 때 우선은 대기 상태로 있는다.
	FVector targetLoc = MainCharacter->GetActorLocation();
	FVector myLoc = me->GetActorLocation();
	FVector dirR = targetLoc - myLoc;
	FRotator rot = dirR.Rotation();

	//NavigationSystem 객체 얻어오기
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	//목적지 길 찾기 경로 데이터 검색
	FPathFindingQuery query;
	FAIMoveRequest req;

	//목적지에서 인지할 수 잇는 범위
	req.SetAcceptanceRadius(500);
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
	// 그렇지 않다면
	else
	{
		//랜덤한 위치로 이동
		auto result = ChaseAI->MoveToLocation(randomPos);
		//목적지에 도착하면
		if (result == EPathFollowingRequestResult::AlreadyAtGoal ||result == EPathFollowingRequestResult::Failed )
		{
			//새로운 랜덤 위치 가져오기
			GetRandomPositionNavMesh(me->GetActorLocation(), 500, randomPos);
		}
		
	}
	
	//너무 가까이 왔을 때 사망 이벤트 발생
	if (dirR.Size() <= 300.0f)
	{
		// 끝나는 엔딩 위젯 나오도록 하기
		if (PlayerHUD && PlayerHUD->DeadEndingWidgetUI && PlayerHUD->YsEvanceUserWidget)
		{
			PlayerHUD->YsEvanceUserWidget->SetVisibility(ESlateVisibility::Hidden);
			PlayerHUD->DeadEndingWidgetUI->SetVisibility(ESlateVisibility::Visible);
			FString name= FString(TEXT("<Red_Big>큐레이터</>"));
			PlayerHUD->DeadEndingWidgetUI->SetRichText_Name(name);
			PlayerHUD->DeadEndingWidgetUI->StartLerpTimer();
		}
	}
	
}

void AChaseStatue::TickAttack(const float& DeltaTime)
{
	//사망 코드 넣기
}

void AChaseStatue::SetState(ChaseStatueState newState)
{
	mState = newState; //상태 지정

	//애니메이션 변경
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
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//플레이어일때
		if (MainCharacter)
		{
			// 끝나는 엔딩 위젯 나오도록 하기
			if (PlayerHUD && PlayerHUD->DeadEndingWidgetUI && PlayerHUD->YsEvanceUserWidget)
			{
				PlayerHUD->YsEvanceUserWidget->SetVisibility(ESlateVisibility::Hidden);
				PlayerHUD->DeadEndingWidgetUI->SetVisibility(ESlateVisibility::Visible);
				FString name= FString(TEXT("<Red_Big>큐레이터</>"));
				PlayerHUD->DeadEndingWidgetUI->SetRichText_Name(name);
				PlayerHUD->DeadEndingWidgetUI->StartLerpTimer();
			}
		}
		
	}
}



