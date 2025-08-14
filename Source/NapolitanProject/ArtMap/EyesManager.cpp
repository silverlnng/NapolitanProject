// Fill out your copyright notice in the Description page of Project Settings.


#include "EyesManager.h"

#include "OriginEye.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SlateWrapperTypes.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/Interact/DigitLockActor.h"

// Sets default values
AEyesManager::AEyesManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);
}

// Called when the game starts or when spawned
void AEyesManager::BeginPlay()
{
	Super::BeginPlay();

	// TriggerBox 영역 안에 있는 OriginEye들만 수집
	TArray<AActor*> OverlappingActors;
	TriggerBox->GetOverlappingActors(OverlappingActors, AOriginEye::StaticClass());

	for (AActor* Actor : OverlappingActors)
	{
		if (AOriginEye* Eye = Cast<AOriginEye>(Actor))
		{
			OriginEyes.Add(Eye);
			Eye->SetEyeVisible(false); // 초기엔 숨김
		}
	}

	TimeElapsed = 0.0f;

	bIsLockOpen = false;
	
	TestPC = GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	if (TestPC)
	{
		MainCharacter =TestPC->GetPawn<ATestCharacter>();
	}

	// 1. "Lock" 태그를 가진 자물쇠 액터를 찾기
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Lock"), FoundActors);

	if (FoundActors.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("LockActor not found!"));
		return;
	}

	DigitLockActor = Cast<ADigitLockActor>(FoundActors[0]);
	UE_LOG(LogTemp, Log, TEXT("Found TargetActor: %s"), *DigitLockActor->GetName());

	// 2. 자물쇠 액터 안의 CameraComponent 중 Tag가 "TargetCamera" 인 것을 찾기
	TArray<UCameraComponent*> CameraComponents;
	DigitLockActor->GetComponents<UCameraComponent>(CameraComponents);

	for (UCameraComponent* CamComp : CameraComponents)
	{
		if (CamComp->ComponentHasTag(FName("TargetCamera")))
		{
			DeathCameraComp = CamComp;
			UE_LOG(LogTemp, Log, TEXT("Found TargetCameraComponent: %s"), *DeathCameraComp->GetName());
		}
		else
		{
			baseCameraComp = CamComp;
		}
	}

	if (!DeathCameraComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetCameraComponent with tag 'TargetCamera' not found!"));
	}

}

// Called every frame
void AEyesManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeElapsed += DeltaTime;

	UpdateEyeVisibility();

	if (TimeElapsed >= TotalTime)
	{
		//SetActorTickEnabled(false); // 더 이상 Tick 안 돌리게
		//눈알 보이는 거 종료

		//제한시간이 끝났을 경우 사망 이벤트 발생
		if(TimeElapsed >= EndTime && bIsLockOpen == false)
		{
			CheckViewingDigitLockActor();
			OnAllEyesRevealed(); // 시간 종료 → 이벤트 실행
		}
	}

}

void AEyesManager::UpdateEyeVisibility()
{
	
	//1분안에 모든 눈알이 보이도록 코드 조정
	float RevealRatio = FMath::Clamp(TimeElapsed / TotalTime, 0.0f, 1.0f);
	int32 RevealCount = FMath::CeilToInt(OriginEyes.Num() * RevealRatio);

	for (int32 i = 0; i < OriginEyes.Num(); ++i)
	{
		bool bShouldBeVisible = i < RevealCount;
		if (OriginEyes[i])
		{
			OriginEyes[i]->SetEyeVisible(bShouldBeVisible);
		}
	}
}

void AEyesManager::OnAllEyesRevealed()
{
	UE_LOG(LogTemp, Warning, TEXT("모든 OriginEye가 나타났습니다!"));
	// 눈알 shake 효과 시간 길었음 좋겠음
	
	// 모든 눈알들을 추격 모드로 전환
	for (AOriginEye* Eye : OriginEyes)
	{
		if (Eye && IsValid(Eye))
		{
			Eye->UpdateChasing(GetWorld()->GetDeltaSeconds());
		}
	}
}

void AEyesManager::CheckViewingDigitLockActor()
{
	if (!CheckViewing)
	{
		CheckViewing=true;
		
		if (DigitLockActor->isInUi)
		{
			baseCameraComp->SetActive(false);
			DeathCameraComp->SetActive(true);
			TestPC->SetViewTargetWithBlend(DigitLockActor,0.1f);
			//위젯도 안보이게 처리하기
			DigitLockActor->DigitLockUi->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

