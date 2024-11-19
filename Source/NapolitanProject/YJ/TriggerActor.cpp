// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerActor.h"

#include "EngineUtils.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/NPC/NPC_Security.h"

// Sets default values
ATriggerActor::ATriggerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);
	BoxComp=CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComponent"));
	BoxComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATriggerActor::BeginPlay()
{
	Super::BeginPlay();
	
	for (TActorIterator<ANPC_Security> It(GetWorld(), ANPC_Security::StaticClass()); It; ++It)
	{
		NPC_Security = *It;
	}
	TestPC = GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	MainCharacter =TestPC->GetPawn<ATestCharacter>();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this,&ATriggerActor::BoxCompBeginOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this,&ATriggerActor::BoxCompEndOverlap);
}

// Called every frame
void ATriggerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATriggerActor::CheckSide()
{
	// 박스의 위치와 로컬 X축 방향을 기준으로 판단
	FVector BoxLocation = GetActorLocation();
	FVector BoxForward = GetActorForwardVector();

	// 캐릭터의 위치를 가져옴
	FVector CharacterLocation = MainCharacter->GetActorLocation();

	// 트리거 박스의 위치에서 캐릭터 위치로 향하는 벡터 계산
	FVector DirectionToCharacter = (CharacterLocation - BoxLocation).GetSafeNormal();

	// 방향 벡터와 트리거 박스의 로컬 X축(Forward Vector)의 외적 계산
	float SideValue = FVector::DotProduct(DirectionToCharacter, BoxForward);

	// 판별 결과 출력 (왼쪽/오른쪽)
	if (SideValue > 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character is on the right side."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Character is on the left side."));
	}
}

void ATriggerActor::BoxCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ATestCharacter::StaticClass()))
	{
		NPC_Security->AudioComp->Play();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "BoxCompBeginOverlap");
		//CheckSide();

		// 타이머로 불끄고 배경음 달라지게 하기
		FTimerHandle Timer;
		GetWorldTimerManager().SetTimer(Timer,[this]()
		{
			
		},3.0f,false);
	}
}

void ATriggerActor::BoxCompEndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(ATestCharacter::StaticClass()))
	{
		NPC_Security->AudioComp->Stop();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "BoxCompEndOverlap");
		//CheckSide();
	}
}
