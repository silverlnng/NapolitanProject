// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitDoor_SecondEnding.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/NapolitanProject.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"

AExitDoor_SecondEnding::AExitDoor_SecondEnding()
{
	BoxComp2 = CreateDefaultSubobject<UBoxComponent>("BoxCompIn");
	BoxComp2->SetupAttachment(ExitDoor);
}

void AExitDoor_SecondEnding::BeginPlay()
{
	Super::BeginPlay();

	//GI =GetGameInstance<UMyTestGameInstance>();
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AExitDoor_SecondEnding::OnBeginOverlap1);
}

void AExitDoor_SecondEnding::OnBeginOverlap1(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	// TSet<int32> AcquireSouvenir; 확인해서 

	if (GI && GI->AcquireSouvenir.Num()<3)
	{
		return;
	}
	
	if(OtherActor)
	{
		if(bIsOpenDoor) // 한번만 열리도록 만든 것
			return;
		
		auto* Target = Cast<ATestCharacter>(OtherActor);
		if(Target)
		{
			bIsOpenDoor = true;
			if (OpenDoorSound)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), OpenDoorSound); // 소리 재생
			}
			
			// 목표 Yaw 계산 (현재 Yaw에서 90도 추가)
			FRotator CurrentRotation = ExitDoor->GetRelativeRotation();
			TargetYaw = CurrentRotation.Yaw + 90.0f;

			BoxComp2->OnComponentBeginOverlap.AddDynamic(this, &AExitDoor_SecondEnding::OnBeginOverlap2);
			// 타이머 설정: 문을 부드럽게 회전
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AExitDoorTrue::RotateDoor, 0.01f, true);

			
		}
	}

	
}

void AExitDoor_SecondEnding::OnBeginOverlap2(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATestCharacter* TestCharacter = Cast<ATestCharacter>(OtherActor);
	if(TestCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s:TestCharacter OnBeginOverla "),*CALLINFO);
		// DetectiveMapState 을 변경 해줘야함
		if (GI)
		{
			GI->DetectiveMapState=EDetectiveMapState::SecondEnding;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s:GI is null "),*CALLINFO);
		}

		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(),DetectiveLevel,true);
		
		// 레벨 이동후
		/*if (DetectiveLevel.IsValid())
		{
			UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(),DetectiveLevel,true);
			UE_LOG(LogTemp,Warning,TEXT("AExitDoor_First:: OpenLevel"))
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s:DetectiveLevel is null "),*CALLINFO);
		}*/
		//  레벨쪽에서 시퀀스 작동되도록함 
	}
}

void AExitDoor_SecondEnding::RotateDoor()
{
	Super::RotateDoor();
}
