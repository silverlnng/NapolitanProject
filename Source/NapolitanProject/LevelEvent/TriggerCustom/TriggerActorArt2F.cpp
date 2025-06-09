// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerActorArt2F.h"
#include "EngineUtils.h"
#include "NapolitanProject/YJ/SoundControlActor.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "Sound/SoundCue.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/Interact/ControllableLightActor.h"
#include "NapolitanProject/Interact/InteractWidget.h"
#include "NapolitanProject/Interact/Sculpture.h"
#include "NapolitanProject/Interact/PieceActor.h"
#include "NapolitanProject/NPC/NPC_Security.h"

// Sets default values
ATriggerActorArt2F::ATriggerActorArt2F()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);
	BoxComp=CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComponent"));
	BoxComp->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void ATriggerActorArt2F::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AControllableLightActor> It(GetWorld(), AControllableLightActor::StaticClass()); It; ++It)
	{
		//AActor* Actor = *It;
		ControllableLightArray.Add(*It);
		
	}
	
	for (TActorIterator<ASoundControlActor> It(GetWorld(), ASoundControlActor::StaticClass()); It; ++It)
	{
		SoundControlActor = *It;
	}
	
	for (TActorIterator<ANPC_Security> It(GetWorld(), ANPC_Security::StaticClass()); It; ++It)
	{
		NPC_Security = *It;
	}
	TestPC = GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	if (TestPC)
	{
		MainCharacter =TestPC->GetPawn<ATestCharacter>();
	}

	BoxComp->OnComponentBeginOverlap.AddDynamic(this,&ATriggerActorArt2F::BoxCompBeginOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this,&ATriggerActorArt2F::BoxCompEndOverlap);
}

// Called every frame
void ATriggerActorArt2F::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATriggerActorArt2F::CheckSide()
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

void ATriggerActorArt2F::BoxCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ATestCharacter::StaticClass()))
	{
		if (NPC_Security)
		{
			NPC_Security->SetState(ESecurityState::Patrol);
			NPC_Security->AudioComp->Play();
			NPC_Security->SetActorTickEnabled(true);
			NPC_Security->PawnSensingComp->SetComponentTickEnabled(true);
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "BoxCompBeginOverlap");
		//CheckSide();

		// 타이머로 불끄고 배경음 달라지게 하기
		if (SoundControlActor&&SoundControlActor->SecondFloor)
		{
			SoundControlActor->BGSoundChange(SoundControlActor->SecondFloor);
		}
		FTimerHandle Timer;
		GetWorldTimerManager().SetTimer(Timer,[this]()
		{
			for (auto light : ControllableLightArray)
			{
				light->TurnOnLight(false);
			}
			
		},3.0f,false);

		
		// 미술관을 탐색하자 ui 제거
		APlayerHUD* PlayerHUD=TestPC->GetHUD<APlayerHUD>();
		if (PlayerHUD&&PlayerHUD->InteractUI)
		{
			FString QuestText =FString(TEXT("미술관을 탐색하자"));
			PlayerHUD->InteractUI->RemoveQuestSlot(QuestText);
		}

		if (MainCharacter)
		{
			// 기존 바인딩 제거
			MainCharacter->OnSpecialInteraction.Clear();

			// 새로운 기능 바인딩
			MainCharacter->OnSpecialInteraction.AddDynamic(this, &ATriggerActorArt2F::Interaction_Art2F);
		}
	}

}

// 2층 아트갤러리에서 나갈때 . 
void ATriggerActorArt2F::BoxCompEndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(ATestCharacter::StaticClass()))
	{
		if (NPC_Security)
		{
			NPC_Security->AudioComp->Stop();
		}
		//플레이어의 심장소리 플레이 중이면 중지
		
		MainCharacter->StopSound();
		
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "BoxCompEndOverlap");
		//CheckSide();
		if (SoundControlActor)
		{
			//SoundControlActor->AudioComp2->FadeOut(3.f,0.f);
			// 2층의 배경음 끄고
			//SoundControlActor->AudioComp1->FadeIn(3.f,1.f);
			// 다시 1층의 배경음을 키다
			//SoundControlActor->bInLobby=true;

			SoundControlActor->SoundChangeOriginLobby();
		}

		if (MainCharacter)
		{
			// 기존 바인딩 제거
			MainCharacter->OnSpecialInteraction.Clear();
		}	
	}
}

void ATriggerActorArt2F::Interaction_Art2F(AActor* Interact)
{
	AControllableLightActor* ControllableLight =Cast<AControllableLightActor>(Interact);
		
	// 라이트라면 라이트로 캐스트해서 
	//현재 조작할 라이트가 있고 그 라이트의 범위 안일떄만 작동
	if (ControllableLight)
	{
		MainCharacter->curControllableLight=ControllableLight;
		//IsLightRangeIn=true;
		// curControllableLight 의 불키는 함수 작동시키기
		if (!MainCharacter->curControllableLight->IsTurnOn)
		{
			MainCharacter->curControllableLight->TurnOnLight(true);
		}
		else
		{
			MainCharacter->curControllableLight->TurnOnLight(false);
		}
	}
	
	ASculpture* Sculpture =Cast<ASculpture>(Interact);
	APieceActor* CurPiece =Cast<APieceActor>(MainCharacter->curItem);
	if (CurPiece&&Sculpture)
	{
		// Sculpture 에서 내려두기 UI 출력
		Sculpture->PutDownPiece(CurPiece);
		MainCharacter->curItem=nullptr;
	}
	
}
