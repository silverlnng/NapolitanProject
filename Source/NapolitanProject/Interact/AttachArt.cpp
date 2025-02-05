// Fill out your copyright notice in the Description page of Project Settings.


#include "AttachArt.h"

#include "SunFlowerKey.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/ArtMap/SunFlowerDoor.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"

// Sets default values
AAttachArt::AAttachArt()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComp->SetupAttachment(SceneComp);

	BoxComp=CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(SceneComp);
}

// Called when the game starts or when spawned
void AAttachArt::BeginPlay()
{
	Super::BeginPlay();

	sunFlowerKey = Cast<ASunFlowerKey>(UGameplayStatics::GetActorOfClass(GetWorld(), ASunFlowerKey::StaticClass()));

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AAttachArt::BeginOverlap);
	StaticMeshComp->SetHiddenInGame(true); //그림 숨기기
	bIsOverlapping = false;
	
}

// Called every frame
void AAttachArt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsOverlapping && sunFlowerKey && sunFlowerKey->bIsPickUp)
	{
		auto* cha = Cast<ATestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (cha)
		{
			DrawDebugBox(
				GetWorld(),
				BoxComp->GetComponentLocation(),
				BoxComp->GetScaledBoxExtent(),
				BoxComp->GetComponentRotation().Quaternion(),
				FColor::White,
				false,  // 지속 시간 (false로 변경하여 Tick마다 갱신)
				-1.0f,
				0,
				3.0f
			);

			FVector PlayerLocation = cha->GetActorLocation();
			float Distance = FVector::Dist(PlayerLocation, GetActorLocation());

			UE_LOG(LogTemp, Display, TEXT("Distance: %f"), Distance);
			
			if (Distance <= 370.0f)
			{
				sunFlowerKey->bIsPickUp = false;
				sunFlowerKey->PutDown();
				StaticMeshComp->SetHiddenInGame(false);
				CuratorDoorOpen();
				bIsOverlapping = false;
			}
		}
	}
}

void AAttachArt::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && Cast<ATestCharacter>(OtherActor))
	{
		bIsOverlapping = true;  // 오버랩 시작 시 플래그 설정
	}
}

void AAttachArt::CuratorDoorOpen()
{
	// 퍼시스턴트 레벨 및 로드된 레벨에서 SunFlowerDoor 찾기
	TArray<AActor*> FoundDoors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASunFlowerDoor::StaticClass(), FoundDoors);

	if (FoundDoors.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No SunFlowerDoor found."));
		return;
	}

	for (AActor* Actor : FoundDoors)
	{
		ASunFlowerDoor* SunFlowerDoor = Cast<ASunFlowerDoor>(Actor);
		if (SunFlowerDoor)
		{
			SunFlowerDoor->bIsOpenKey = true;

			// 문 열림 상태 확인 로그
			UE_LOG(LogTemp, Warning, TEXT("SunFlowerDoor is open"));
			UE_LOG(LogTemp, Warning, TEXT("bIsOpenKey: %s"), SunFlowerDoor->bIsOpenKey ? TEXT("True") : TEXT("False"));
			
		}
	}

}




