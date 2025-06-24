// Fill out your copyright notice in the Description page of Project Settings.


#include "Spector.h"

#include "AIController.h"
#include "SpectatorAnim.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpector::ASpector()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpector::BeginPlay()
{
	Super::BeginPlay();

	AI = Cast<AAIController>(GetController());
	bIsMoving = false;
	Anim = Cast<USpectatorAnim>(GetMesh()->GetAnimInstance());
	
}

// Called every frame
void ASpector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//현재 상태 뜨게 하기
	FString myState = UEnum::GetValueAsString(mState);
	DrawDebugString(GetWorld() , GetActorLocation() , myState , nullptr , FColor::Yellow , 0 , true , 1);

	switch (mState)
	{
	case ESpectorState::Watch:
		TickWatch(DeltaTime);
		break;
	case ESpectorState::Move:
		TickMove(DeltaTime);
		break;
	}

}

// Called to bind functionality to input
void ASpector::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASpector::SetState(ESpectorState newstate)
{
	mState = newstate;

	//애니메이션 관련 함수 넣기
	if(Anim)
	{
		Anim->animState = newstate;
	}
}

void ASpector::TickWatch(const float& DeltaTime)
{
	//그림 앞에 도달 시 멈추고 그림을 바라봄
	// 대기 시간 계산
	CurrentTime += DeltaTime;
	if (CurrentTime > IdleDelayTime)
	{

		//그림 앞에 도착했을 시 멈춰서 그림 바라봄 => 액터를 찾아서 그 위치로 이동하게 하기
		// 월드에서 "art" 태그를 가진 모든 액터 찾기
		TArray<AActor*> ArtActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("art"), ArtActors);
    
		if (ArtActors.Num() > 0)
		{
			// 아트 액터들의 위치를 FVector 배열로 변환
			TArray<FVector> ArtPoints;
			for (AActor* ArtActor : ArtActors)
			{
				if (ArtActor)
				{
					ArtPoints.Add(ArtActor->GetActorLocation());
				}
			}
        
			// 마지막 방문한 위치 제외하고 랜덤으로 선택
			if (ArtPoints.Contains(LastVisitedPoint))
			{
				ArtPoints.Remove(LastVisitedPoint);
			}
        
			if (ArtPoints.Num() > 0)
			{
				int32 randomIndex = FMath::RandRange(0, ArtPoints.Num() - 1);
            
				// 제거된 위치를 다시 추가
				if (LastVisitedPoint != FVector::ZeroVector)
				{
					ArtPoints.Add(LastVisitedPoint);
				}
            
				TargetPoint = ArtPoints[randomIndex];
				LastVisitedPoint = TargetPoint;
				
				// Move 상태로 전환
				SetState(ESpectorState::Move);
				CurrentTime = 0; // 시간 초기화
				bIsMoving = true;
            
				UE_LOG(LogTemp, Warning, TEXT("New art target point set: X=%f, Y=%f, Z=%f"), 
					   TargetPoint.X, TargetPoint.Y, TargetPoint.Z);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No art actors found with 'art' tag"));
		}
	}
	
}

void ASpector::TickMove(const float& DeltaTime)
{
    if (AI && bIsMoving == true)
    {
        AI->MoveToLocation(TargetPoint);
        
        if (FVector::Dist(GetActorLocation(), TargetPoint) <= 100.f)
        {
            AI->StopMovement();
            bIsMoving = false;
            
            // 가장 가까운 아트 작품 찾기
            TArray<AActor*> ArtActors;
            UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("art"), ArtActors);
            
            AActor* ClosestArt = nullptr;
            float ClosestDistance = FLT_MAX;
            
            for (AActor* ArtActor : ArtActors)
            {
                if (ArtActor)
                {
                    float Distance = FVector::Dist(GetActorLocation(), ArtActor->GetActorLocation());
                    if (Distance < ClosestDistance)
                    {
                        ClosestDistance = Distance;
                        ClosestArt = ArtActor;
                    }
                }
            }
            
            if (ClosestArt)
            {
                UArrowComponent* ArrowComp = ClosestArt->FindComponentByClass<UArrowComponent>();
                
                if (ArrowComp)
                {
                    FVector ArrowForward = ArrowComp->GetForwardVector();
                    ArrowForward.Z = 0;
                    
                    if (!ArrowForward.IsNearlyZero())
                    {
                        TargetRotation = ArrowForward.Rotation();
                        bIsRotating = true;
                    }
                }
            }
        }
    }
    
    // 부드러운 회전 처리
    if (bIsRotating)
    {
        FRotator CurrentRotation = GetActorRotation();
        FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);
        SetActorRotation(NewRotation);
        
        if (FMath::IsNearlyEqual(NewRotation.Yaw, TargetRotation.Yaw, 5.0f))
        {
            bIsRotating = false;
            SetState(ESpectorState::Watch);
        }
    }
}
