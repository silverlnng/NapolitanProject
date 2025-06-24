// Fill out your copyright notice in the Description page of Project Settings.


#include "Spector.h"

#include "AIController.h"
#include "SpectatorAnim.h"
#include "SpectorManager.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

class ASpectorManager;
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

// 수정된 TickWatch 함수
void ASpector::TickWatch(const float& DeltaTime)
{
    // 대기 시간 계산
    CurrentTime += DeltaTime;
    if (CurrentTime > IdleDelayTime)
    {
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
            
            // 매니저를 통해 사용 가능한 위치 가져오기
            ASpectorManager* Manager = ASpectorManager::GetInstance(GetWorld());
            if (Manager)
            {
                // 이전에 예약한 위치가 있다면 해제
                if (CurrentReservedPosition != FVector::ZeroVector)
                {
                    Manager->ReleaseArtPosition(CurrentReservedPosition);
                    CurrentReservedPosition = FVector::ZeroVector;
                }
                
                FVector NewTargetPoint = Manager->GetAvailableArtPosition(this, ArtPoints);
                
                if (NewTargetPoint != FVector::ZeroVector)
                {
                    // 새 위치 예약
                    if (Manager->ReserveArtPosition(NewTargetPoint, this))
                    {
                        TargetPoint = NewTargetPoint;
                        LastVisitedPoint = TargetPoint;
                        CurrentReservedPosition = TargetPoint;
                        
                        // Move 상태로 전환
                        SetState(ESpectorState::Move);
                        CurrentTime = 0;
                        bIsMoving = true;
                        
                        UE_LOG(LogTemp, Warning, TEXT("Spector %s reserved art position: X=%f, Y=%f, Z=%f"), 
                               *GetName(), TargetPoint.X, TargetPoint.Y, TargetPoint.Z);
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Failed to reserve art position for %s"), *GetName());
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("No available art positions for %s"), *GetName());
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("SpectorManager not found!"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No art actors found with 'art' tag"));
        }
    }
}

// 수정된 TickMove 함수 - 도착 시 예약 해제 추가
void ASpector::TickMove(const float& DeltaTime)
{
    if (AI && bIsMoving == true)
    {
        AI->MoveToLocation(TargetPoint);
        
        if (FVector::Dist(GetActorLocation(), TargetPoint) <= 100.f)
        {
            AI->StopMovement();
            bIsMoving = false;
            
            // 가장 가까운 아트 작품 찾기 및 회전 로직 (기존과 동일)
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

// Spector가 파괴될 때 예약 해제
void ASpector::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // 예약된 위치 해제
    if (CurrentReservedPosition != FVector::ZeroVector)
    {
        ASpectorManager* Manager = ASpectorManager::GetInstance(GetWorld());
        if (Manager)
        {
            Manager->ReleaseArtPosition(CurrentReservedPosition);
        }
    }
    
    Super::EndPlay(EndPlayReason);
}
