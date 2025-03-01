// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_LeeSeo.h"

#include "Components/CapsuleComponent.h"
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
		Destroy(); //얻으면 삭제
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
				
				//FVector ForwardDirection = GetActorForwardVector(); // 캐릭터의 전방 벡터
				//FVector LaunchVelocity = ForwardDirection * 600.0f; // 속도 조절 (600 = 이동 속도)
				//LaunchCharacter(LaunchVelocity, true, false); // X, Y 방향 이동 가능. Z(점프)는 비활성화

				//앞으로 달리는 듯한 애니메이션 몽타주 재생 -> 점프스케어
				
			}, 1.0f, false);

			
			GetWorldTimerManager().SetTimer(JumpSquareTimerHandle, [this]()
			{
				SpawnItem();
			}, 4.0f, false);
		}
		else if( 1 == result)
		{
			//1-2. 그림을 찢어선 안돼 => 맵에서 갑자기 소름 돋는 노래 재생.
			UGameplayStatics::PlaySound2D(GetWorld(), LSJump);
			
			//대사창 5초 뒤 사라지고 그와 동시에 전구가 깜빡거림

			
			//이서 1초 사라졌다가 달려오는 점프 스케어 발동
			FTimerHandle TimeHandle;
			GetWorldTimerManager().SetTimer(TimeHandle, [this]()
			{
				//일시적으로 안보이게 하기
				GetMesh()->SetHiddenInGame(false);
				GetComponentByClass<UCapsuleComponent>()->SetCollisionProfileName(FName("ClearedNPC"));
			}, 2.0f, false);

			GetWorldTimerManager().SetTimer(JumpSquareTimerHandle, [this]()
			{
				GetMesh()->SetHiddenInGame(true);
				
				//앞으로 달리는 애니메이션 재생 : 칼 등의 요소 들고 달리기
				
			}, 3.0f, false);

			//점프 스케어 후 사라짐
			GetMesh()->SetHiddenInGame(false);
			
		}
		else if( 2 == result)
		{
			//1-3. 도망친다 => 맵에서 갑자기 소름 돋는 노래 재생.
			UGameplayStatics::PlaySound2D(GetWorld(), LSJump);
			
			//전구가 깜빡거림

			//이서 1초 사라졌다가 달려오는 점프 스케어 발동
			FTimerHandle TimeHandle;
			GetWorldTimerManager().SetTimer(TimeHandle, [this]()
			{
				//일시적으로 안보이게 하기
				GetMesh()->SetHiddenInGame(false);
				GetComponentByClass<UCapsuleComponent>()->SetCollisionProfileName(FName("ClearedNPC"));
			}, 2.0f, false);

			GetWorldTimerManager().SetTimer(JumpSquareTimerHandle, [this]()
			{
				GetMesh()->SetHiddenInGame(true);
				
				//앞으로 달리는 애니메이션 재생 : 칼 등의 요소 들고 달리기
				
			}, 3.0f, false);

			//점프 스케어 후 사라짐
			GetMesh()->SetHiddenInGame(false);

			//사망 UI 재생
		}
	}
	
}
