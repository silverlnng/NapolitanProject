// Fill out your copyright notice in the Description page of Project Settings.


#include "AttachArt.h"

#include "SunFlowerKey.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
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
	StaticMeshComp->SetHiddenInGame(true);
}

// Called every frame
void AAttachArt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAttachArt::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		auto* cha=Cast<ATestCharacter>(OtherActor);
		//만약 플레이어가 그림을 들고 있으면 -> 라인 박스 켜짐
		if(sunFlowerKey->bIsPickUp && cha)
		{
			DrawDebugBox(
			   GetWorld(),
			   BoxComp->GetComponentLocation(),
			   BoxComp->GetScaledBoxExtent(),
			   BoxComp->GetComponentRotation().Quaternion(),
			   FColor::White,
			   true,  // 지속 시간
			   -1.0f, // 무한 지속
			   0,     // 두께
			   3.0f   // 선 두께
			);
			
			FVector PlayerLocation = cha->GetActorLocation();
			float Distance = FVector::Dist(PlayerLocation, GetActorLocation());
			
			if(Distance <= 350.0f){
				//플레이어의 거리가 가까울 경우 벽에 부착 가능
				sunFlowerKey->bIsPickUp = false;
				sunFlowerKey->PutDown(); //아이템 없애기
				StaticMeshComp->SetHiddenInGame(true);
			}
		}
	}	
}



