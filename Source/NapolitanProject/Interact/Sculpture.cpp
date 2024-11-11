// Fill out your copyright notice in the Description page of Project Settings.


#include "Sculpture.h"

#include "ItemActor.h"
#include "Components/BoxComponent.h"

// Sets default values
ASculpture::ASculpture()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);

	BoxComp=CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComponent"));
	BoxComp->SetupAttachment(RootComponent);

	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,ECR_Block);

	StaticMeshComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComp->SetupAttachment(BoxComp);

	SceneComp1 =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp1"));
	SceneComp1->SetupAttachment(StaticMeshComp);

	SceneComp2 =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp2"));
	SceneComp2->SetupAttachment(StaticMeshComp);

	SceneComp3 =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp3"));
	SceneComp3->SetupAttachment(StaticMeshComp);

}

// Called when the game starts or when spawned
void ASculpture::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASculpture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASculpture::PutDownPiece(AItemActor* Piece)
{
	// 지금 현재 가진게 첫번째 조각이면 첫번째 위치에 두기
		// piece Num =1이라면
	int32 PieceNum=Piece->ItemID;
	switch (PieceNum)
	{
	case 1:
		// 케릭터가 가진 현재조각을
			Piece->SetActorLocation(SceneComp1->GetComponentLocation());
			PieceArray.Add(Piece);
		break;
	case 2:
		Piece->SetActorLocation(SceneComp2->GetComponentLocation());
		PieceArray.Add(Piece);
		break;
	case 3:
		Piece->SetActorLocation(SceneComp3->GetComponentLocation());
		PieceArray.Add(Piece);
		break;	
	}
	// 3개 모두 다모이면 미션이 성공된걸 알리도록 하기

	//PieceArray 의 크기가 3이면 미션 완료
	if (PieceArray.Num()==3)
	{
		MissionCheck();
	}
}

void ASculpture::MissionCheck()
{
	// 미션수행할때 부르는 함수
	// 
}

