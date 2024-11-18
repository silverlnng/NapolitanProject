// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_LeeSeo.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ANPC_LeeSeo::ANPC_LeeSeo()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANPC_LeeSeo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPC_LeeSeo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bJumpSquare)
	{
		//점프 스케어
		
	}

}

void ANPC_LeeSeo::Interact()
{
	Super::Interact();
}

int32 ANPC_LeeSeo::GetNPCID()
{
	return NPC_ID;
}

int32 ANPC_LeeSeo::GetState()
{
	return State;
}

void ANPC_LeeSeo::ChangeCleared()
{
	Super::ChangeCleared();
}

void ANPC_LeeSeo::SpawnItem()
{
	// 그림 앞의 위치를 기준으로 스폰 위치 설정
	AActor* LeeseoPointActor = UGameplayStatics::GetActorOfClass(GetWorld(), LeeseoPoint);
	FVector SpawnLocation =  LeeseoPointActor->GetActorLocation();
	FTransform SpawnTransform(SpawnLocation);

	AActor* SouvenirActor = GetWorld()->SpawnActor<ASouvenirActor>(SouvenirClass, SpawnTransform );
	if (SouvenirActor)
	{
		SouvenirActor->Tags.Add(FName("Souvenir"));
	}
	
}

void ANPC_LeeSeo::ResultEvent(int32 result)
{
	//결과 이벤트 작성
	if(1== State)
	{
		if(0 == result)
		{
			//귀를 막고 외친다. "너는 거짓말쟁이야" => 유품 획득
			//소녀가 비명을 지른다. -> 점프스퀘어 발동 후 사라짐 그림앞에 유품 스폰
			ChangeCleared();
			UGameplayStatics::PlaySound2D(GetWorld(), ScreamSound); //소리 지르기

			//그와 동시에 불빛이 흔들림

			//이서가 180도 회전
			FRotator NewRotation = GetActorRotation();
			NewRotation.Roll = 180.0f;
			SetActorRotation(NewRotation);

			GetWorldTimerManager().SetTimer(JumpSquareTimerHandle, [this]()
			{
				//앞으로 달리기
				FVector ForwardDirection = GetActorForwardVector(); // 캐릭터의 전방 벡터
				FVector LaunchVelocity = ForwardDirection * 600.0f; // 속도 조절 (600 = 이동 속도)
				LaunchCharacter(LaunchVelocity, true, false); // X, Y 방향 이동 가능. Z(점프)는 비활성화
			}, 1.0f, false);

			
			GetWorldTimerManager().SetTimer(JumpSquareTimerHandle, [this]()
			{
				SpawnItem();
			}, 4.0f, false);
		}
	}
	
}
