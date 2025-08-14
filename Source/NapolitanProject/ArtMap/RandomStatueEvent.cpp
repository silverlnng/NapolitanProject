// RandomStatueEvent.cpp
#include "RandomStatueEvent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ARandomStatueEvent::ARandomStatueEvent()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    // 루트 컴포넌트로 BoxComponent 생성
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    RootComponent = BoxComponent;
}

// Called when the game starts or when spawned
void ARandomStatueEvent::BeginPlay()
{
    Super::BeginPlay();

    // 오버랩 이벤트 바인딩
    if (BoxComponent)
    {
        BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ARandomStatueEvent::OnTriggerBeginOverlap);
    }
    
    // 원본 메쉬 가시성 저장
    OriginalMeshVisibility.Reserve(MeshesToHide.Num());
    for (AActor* MeshActor : MeshesToHide)
    {
        if (IsValid(MeshActor))
        {
            UStaticMeshComponent* MeshComp = MeshActor->FindComponentByClass<UStaticMeshComponent>();
            if (MeshComp)
            {
                OriginalMeshVisibility.Add(MeshComp->GetVisibleFlag());
            }
            else
            {
                OriginalMeshVisibility.Add(true);
            }
        }
        else
        {
            OriginalMeshVisibility.Add(true);
        }
    }
    
    // 원본 조각상 회전 저장
    OriginalStatueRotations.Reserve(StatuesToRotate.Num());
    for (AActor* StatueActor : StatuesToRotate)
    {
        if (IsValid(StatueActor))
        {
            OriginalStatueRotations.Add(StatueActor->GetActorRotation());
        }
        else
        {
            OriginalStatueRotations.Add(FRotator::ZeroRotator);
        }
    }
}

// Called every frame
void ARandomStatueEvent::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ARandomStatueEvent::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 플레이어 캐릭터인지 확인
    if (!IsValid(OtherActor) || !OtherActor->IsA(ACharacter::StaticClass()))
    {
        return;
    }
    
    // 이미 트리거된 상태라면 무시
    if (bIsTriggered)
    {
        return;
    }
    
    // 랜덤 이벤트 실행
    ExecuteRandomEvent();
}

void ARandomStatueEvent::ExecuteRandomEvent()
{
    // 사용 가능한 이벤트 타입 결정
    bool bHasMeshesToHide = (MeshesToHide.Num() > 0);
    bool bHasStatuesToRotate = (StatuesToRotate.Num() > 0);
    
    if (!bHasMeshesToHide && !bHasStatuesToRotate)
    {
        return;
    }
    
    // 둘 다 실행 (사용 가능한 이벤트들)
    if (bHasMeshesToHide)
    {
        ExecuteHideMeshesEvent();
    }
    
    if (bHasStatuesToRotate)
    {
        ExecuteRotateStatueEvent();
    }
    
    // 트리거 상태 설정
    bIsTriggered = true;
    
    // 재트리거 가능하다면 타이머 설정
    if (bCanRetrigger)
    {
        GetWorld()->GetTimerManager().SetTimer(
            RetriggerTimerHandle,
            this,
            &ARandomStatueEvent::ResetTrigger,
            RetriggerDelay,
            false
        );
    }
}

void ARandomStatueEvent::ExecuteHideMeshesEvent()
{
    for (int32 i = 0; i < MeshesToHide.Num(); ++i)
    {
        AActor* MeshActor = MeshesToHide[i];
        if (!IsValid(MeshActor))
        {
            continue;
        }
        
        // StaticMeshComponent 찾기
        UStaticMeshComponent* MeshComp = MeshActor->FindComponentByClass<UStaticMeshComponent>();
        if (MeshComp)
        {
            // 메쉬 숨기기
            MeshComp->SetVisibility(false);
            
            // 디버그 메시지
            
            // 필요하다면 충돌도 비활성화
            MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
        else
        {
            // StaticMeshComponent가 없다면 전체 액터 숨기기
            MeshActor->SetActorHiddenInGame(true);
        }
    }
}

void ARandomStatueEvent::ExecuteRotateStatueEvent()
{
    
    for (int32 i = 0; i < StatuesToRotate.Num(); ++i)
    {
        AActor* StatueActor = StatuesToRotate[i];
        
        if (!IsValid(StatueActor))
        {
            continue;
        }
        
        // 현재 회전값 가져오기
        FRotator CurrentRotation = StatueActor->GetActorRotation();
        
        // 30~90도 사이의 랜덤한 각도 생성
        float RandomRotationAngle = FMath::RandRange(MinRotationAngle, MaxRotationAngle);
        
        // 시계방향/반시계방향 랜덤 결정 (50% 확률)
        if (FMath::RandBool())
        {
            RandomRotationAngle = -RandomRotationAngle;
        }
        // 새로운 회전값 계산 (Z축 회전)
        FRotator NewRotation = CurrentRotation;
        NewRotation.Yaw += RandomRotationAngle;
        
        // 즉시 회전 적용
        StatueActor->SetActorRotation(NewRotation);
        
        // 적용 후 실제 회전값 확인
        FRotator ActualRotation = StatueActor->GetActorRotation();
        
    }
}

void ARandomStatueEvent::ResetTrigger()
{
    bIsTriggered = false;
    
    // 필요하다면 여기서 메쉬들을 다시 보이게 하거나 조각상을 원래 위치로 되돌릴 수 있습니다
    // 현재는 한 번 실행된 이벤트는 유지되도록 구현
}