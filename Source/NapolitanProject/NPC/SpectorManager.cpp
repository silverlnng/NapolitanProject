// SpectorManager.cpp
#include "SpectorManager.h"
#include "Spector.h"
#include "Kismet/GameplayStatics.h"

// Static 멤버 변수 정의 (이 부분이 빠져있었음!)
ASpectorManager* ASpectorManager::Instance = nullptr;

ASpectorManager::ASpectorManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ASpectorManager::BeginPlay()
{
    Super::BeginPlay();
    Instance = this;
}

void ASpectorManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

ASpectorManager* ASpectorManager::GetInstance(UWorld* World)
{
    if (!Instance && World)
    {
        // 기존 매니저 찾기
        TArray<AActor*> FoundManagers;
        UGameplayStatics::GetAllActorsOfClass(World, ASpectorManager::StaticClass(), FoundManagers);
        
        if (FoundManagers.Num() > 0)
        {
            Instance = Cast<ASpectorManager>(FoundManagers[0]);
        }
        else
        {
            // 매니저가 없으면 새로 생성
            Instance = World->SpawnActor<ASpectorManager>();
        }
    }
    return Instance;
}

bool ASpectorManager::ReserveArtPosition(FVector Position, ASpector* Spector)
{
    // 이미 점유된 위치인지 확인
    if (OccupiedArtPositions.Contains(Position))
    {
        return false; // 이미 점유됨
    }
    
    // 위치 예약
    OccupiedArtPositions.Add(Position, Spector);
    UE_LOG(LogTemp, Log, TEXT("Art position reserved by %s"), *Spector->GetName());
    return true;
}

void ASpectorManager::ReleaseArtPosition(FVector Position)
{
    if (OccupiedArtPositions.Contains(Position))
    {
        OccupiedArtPositions.Remove(Position);
        UE_LOG(LogTemp, Log, TEXT("Art position released"));
    }
}

FVector ASpectorManager::GetAvailableArtPosition(ASpector* RequestingSpector, const TArray<FVector>& ArtPositions)
{
    TArray<FVector> AvailablePositions;
    
    // 점유되지 않은 위치들만 필터링
    for (const FVector& Position : ArtPositions)
    {
        if (!OccupiedArtPositions.Contains(Position))
        {
            AvailablePositions.Add(Position);
        }
    }
    
    // 마지막 방문 위치 제외
    if (AvailablePositions.Contains(RequestingSpector->LastVisitedPoint))
    {
        AvailablePositions.Remove(RequestingSpector->LastVisitedPoint);
    }
    
    // 사용 가능한 위치가 없으면 점유된 위치도 포함 (모든 위치가 점유된 경우)
    if (AvailablePositions.Num() == 0)
    {
        AvailablePositions = ArtPositions;
        if (AvailablePositions.Contains(RequestingSpector->LastVisitedPoint))
        {
            AvailablePositions.Remove(RequestingSpector->LastVisitedPoint);
        }
    }
    
    if (AvailablePositions.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, AvailablePositions.Num() - 1);
        return AvailablePositions[RandomIndex];
    }
    
    return FVector::ZeroVector;
}