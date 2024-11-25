// Fill out your copyright notice in the Description page of Project Settings.


#include "Sculpture.h"

#include "ClueActor.h"
#include "NiagaraComponent.h"
#include "EngineUtils.h"
#include "ItemActor.h"
#include "PieceActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/EventComponent.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/NPC/NPC_Security.h"

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

	SkeletalMeshComp=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	SkeletalMeshComp->SetupAttachment(BoxComp);
	
	StaticMeshComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComp->SetupAttachment(BoxComp);

	SceneComp1 =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp1"));
	SceneComp1->SetupAttachment(StaticMeshComp);

	SceneComp2 =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp2"));
	SceneComp2->SetupAttachment(StaticMeshComp);

	SceneComp3 =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp3"));
	SceneComp3->SetupAttachment(StaticMeshComp);

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
	NiagaraComp->SetupAttachment(StaticMeshComp);
	NiagaraComp2 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp2"));
	NiagaraComp2->SetupAttachment(StaticMeshComp);
	NiagaraComp3 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp3"));
	NiagaraComp3->SetupAttachment(StaticMeshComp);
	
}

// Called when the game starts or when spawned
void ASculpture::BeginPlay()
{
	Super::BeginPlay();
	TestPC=GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	for (TActorIterator<ANPC_Security> It(GetWorld(), ANPC_Security::StaticClass()); It; ++It)
	{
		NPC_Security = *It;
	}
	
}

// Called every frame
void ASculpture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASculpture::PutDownPiece(APieceActor* Piece)
{
	// 지금 현재 가진게 첫번째 조각이면 첫번째 위치에 두기
		// piece Num =1이라면
	Piece->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	int32 PieceNum=Piece->PieceNum;
	switch (PieceNum)
	{
	case 1:
		// 케릭터가 가진 현재조각을
			Piece->AttachToComponent(SceneComp1,FAttachmentTransformRules::SnapToTargetNotIncludingScale);	
			PieceArray.Add(Piece);
		break;
	case 2:
		Piece->AttachToComponent(SceneComp2,FAttachmentTransformRules::SnapToTargetNotIncludingScale);	
		PieceArray.Add(Piece);
		break;
	case 3:
		Piece->AttachToComponent(SceneComp3,FAttachmentTransformRules::SnapToTargetNotIncludingScale);	
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
	UE_LOG(LogTemp,Warning,TEXT("경비원 미션 달성"));
	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,ECR_Ignore);

	// 소리나오도록 하기
	if (MissionClearSoundWave)
	{
		UGameplayStatics::PlaySound2D(this, MissionClearSoundWave);
	}
	// 배경음 바꾸기 
	

	// 경비원으로 카메라 전환한뒤 사라지고 머리 남기는걸 보여 주기
	if (NPC_Security)
	{
		NPC_Security->EndEvent();
	}
	
	// 월드에 있는 경비원 CLEAR으로 변경

	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer,[this]()
	{

		// 피흘리도록하기
		NiagaraComp->Activate();
		NiagaraComp2->Activate();
		NiagaraComp3->Activate();
		// 피가 아예 묻도록 하기 UGameplayStatics::SpawnDecalAtLocation

		// 경비원 노트 ui 팝업되도록 만들기
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FVector SpawnLocation = SceneComp2->GetComponentLocation();
		FRotator tempRot =FRotator(0.f,90.f,0.f);
		if (Clue10)
		{
			GetWorld()->SpawnActor<AClueActor>(Clue10,SpawnLocation,tempRot,SpawnParams);
		}
		// 단서가 나오도록 하기
		//

		// 그단서 
		
	},11.5f,false);

	FTimerHandle Timer2;
	GetWorldTimerManager().SetTimer(Timer2,[this]()
	{
		if (TestPC)
		{
			TestPC->EventComponent->Event_Security_Completed();
		}

	},14.f,false);

	
}

