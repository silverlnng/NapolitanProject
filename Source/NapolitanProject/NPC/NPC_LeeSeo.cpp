// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_LeeSeo.h"

#include "EngineUtils.h"
#include "NPC_LeeSeoAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/ArtMap/SequentialLightController.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/LevelEvent/LightControlActor.h"
#include "NapolitanProject/YJ/DeadEndingWidget.h"

class ALightControlActor;
// Sets default values
ANPC_LeeSeo::ANPC_LeeSeo()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 몬스터 카메라 생성 및 초기화
	MonsterCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MonsterCamera"));
	MonsterCamera->SetupAttachment(GetMesh(),FName(TEXT("HeadSocket"))); // 루트에 부착
	MonsterCamera->bUsePawnControlRotation = false; // 플레이어 조작 방지

}

// Called when the game starts or when spawned
void ANPC_LeeSeo::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ASequentialLightController> It(GetWorld(), ASequentialLightController::StaticClass()); It; ++It)
	{
		LightControlReference = *It;
	}

	Anim = Cast<UNPC_LeeSeoAnimInstance>(GetMesh()->GetAnimInstance());
	
}

// Called every frame
void ANPC_LeeSeo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 이동 로직
	if (bShouldMove && MoveTimeRemaining > 0.0f)
	{
		// 앞으로 이동 입력
		AddMovementInput(GetActorForwardVector(), MoveSpeed);
        
		// 남은 시간 감소
		MoveTimeRemaining -= DeltaTime;
        
		// 이동 종료 확인
		if (MoveTimeRemaining <= 0.0f)
		{
			bShouldMove = false;
			UE_LOG(LogTemp, Warning, TEXT("Movement completed"));
		}
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
	IsCleared=true;
	GetComponentByClass<UCapsuleComponent>()->SetCollisionProfileName(FName("ClearedNPC"));
	GI->ClearedNPC.Add(GetNPCID());
}

void ANPC_LeeSeo::SwitchToMonsterCamera()
{
	//이서의 점프스케어 카메라로 변경되는 함수
	if(TestPC && MonsterCamera)
	{
		// 1. 먼저 이 액터를 ViewTarget으로 설정
		TestPC->SetViewTargetWithBlend(this, 0.01f);
            
		// 2. 그 다음 액터의 활성 카메라를 MonsterCamera로 변경
		SetActorViewTarget(MonsterCamera);

		//3. 플레이어 메쉬 숨기기
		MainCharacter->GetMesh()->SetHiddenInGame(true);

		// 카메라 흔들기 실행
		if (TestPC->PlayerCameraManager)
		{
			if (DeathCameraShakeClass)
			{
				TestPC->PlayerCameraManager->StartCameraShake(DeathCameraShakeClass);
				
			}
		}
	}

	FTimerHandle UITimer2;
	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this]()
	{
		if (PlayerHUD )
		{
		
			PlayerHUD->PlayDeadVignetteEffect();
			UE_LOG(LogTemp, Warning, TEXT("CreateDieUI1"));
		}
	},1.5f,false);
	
	//시간 지연 주고 사망 UI 나오도록
	FTimerHandle UITimer;
	GetWorldTimerManager().SetTimer(UITimer, [this]()
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateDieUI2"));
		MainCharacter->SetPlayerState(EPlayerState::UI);

		if(PlayerHUD && PlayerHUD->DeadEndingWidgetUI)
		{
			PlayerHUD->DeadEndingWidgetUI->SetVisibility(ESlateVisibility::Visible);
			PlayerHUD->DeadEndingWidgetUI->SetTextBlock_description(description);
		}
	},2.5f,false); //사망
	
}

void ANPC_LeeSeo::StartMovingForward(float Duration, float Speed)
{
	bShouldMove = true;
	MoveTimeRemaining = Duration;
	MoveSpeed = Speed;
}

void ANPC_LeeSeo::ExecuteJumpScareSequence()
{
	// 1초 후: 일시적으로 안보이게 하기
	FTimerHandle Step1TimerHandle;
	GetWorldTimerManager().SetTimer(Step1TimerHandle, this, &ANPC_LeeSeo::HideMesh, 2.5f, false);
}

void ANPC_LeeSeo::HideMesh()
{
	UE_LOG(LogTemp, Warning, TEXT("HideMesh"));
	GetMesh()->SetHiddenInGame(true);

	// Yaw(Z축)에 20도 더하기
	AddActorLocalRotation(FRotator(0.0f, 20.0f, 0.0f));
    
	// 2초 후: 다시 보이게 하고 달리는 애니메이션
	FTimerHandle Step2TimerHandle;
	GetWorldTimerManager().SetTimer(Step2TimerHandle, this, &ANPC_LeeSeo::RunAnim, 2.0f, false);
}

void ANPC_LeeSeo::RunAnim()
{
	//맵에 설치되어있던 BP_BoxCollision을 찾아서 제거
	RemoveBPBoxCollision();
	
	UE_LOG(LogTemp, Warning, TEXT("RunAnim"));
	GetMesh()->SetHiddenInGame(false);

	// 지속적인 이동 시작 (2초 동안)
	StartMovingForward(0.5f, 1.0f);
    
	//앞으로 달리는 애니메이션 재생 : 칼 등의 요소 들고 달리기
	if(Anim)
	{
		Anim->PlayJumpSkareMontage2();
	}
    
	// 1초 후: 카메라 전환 및 공격 애니메이션
	FTimerHandle Step3TimerHandle;
	GetWorldTimerManager().SetTimer(Step3TimerHandle, this, &ANPC_LeeSeo::AttackScare, 0.5f, false);
}

void ANPC_LeeSeo::AttackScare()
{

	//칼로 찌르는 몽타주 재생
	if(Anim)
	{
		Anim->PlayJumpSkareMontage3();
		UE_LOG(LogTemp, Warning, TEXT("PlayJumpSkareMontage3"));
	}

	FTimerHandle CameraTimer;
	GetWorld()->GetTimerManager().SetTimer(CameraTimer,[this]()
	{
		//점프 스케어 카메라 전환
		SwitchToMonsterCamera();
	},0.75f,false);
	
    
	// 2.0초 후: 사망 UI 표시
	//FTimerHandle Step4TimerHandle;
	//GetWorldTimerManager().SetTimer(Step4TimerHandle, this, &ANPC_LeeSeo::CreateDieUI, 2.0f, false);
}

void ANPC_LeeSeo::CreateDieUI()
{
	
	FTimerHandle UITimer2;
	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this]()
	{
		if (PlayerHUD )
		{
		
			PlayerHUD->PlayDeadVignetteEffect();
			UE_LOG(LogTemp, Warning, TEXT("CreateDieUI1"));
		}
	},2.5f,false);
	
	//시간 지연 주고 사망 UI 나오도록
	FTimerHandle UITimer;
	GetWorldTimerManager().SetTimer(UITimer, [this]()
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateDieUI2"));
		MainCharacter->SetPlayerState(EPlayerState::UI);

		if(PlayerHUD && PlayerHUD->DeadEndingWidgetUI)
		{
			PlayerHUD->DeadEndingWidgetUI->SetVisibility(ESlateVisibility::Visible);
			PlayerHUD->DeadEndingWidgetUI->SetTextBlock_description(description);
		}
	},3.5f,false); //사망
}

void ANPC_LeeSeo::SetActorViewTarget(UCameraComponent* TargetCamera)
{
	if (TargetCamera)
	{
		// 다른 카메라들 비활성화
		TArray<UCameraComponent*> CameraComponents;
		GetComponents<UCameraComponent>(CameraComponents);
        
		for (UCameraComponent* Camera : CameraComponents)
		{
			Camera->SetActive(false);
		}
        
		// MonsterCamera만 활성화
		TargetCamera->SetActive(true);
	}
}


void ANPC_LeeSeo::SpawnItem()
{
	// 그림 앞의 위치를 기준으로 스폰 위치 설정
	FVector SpawnLocation = GetMesh()->GetSocketLocation(FName("ItemSpawn"));;
	FTransform SpawnTransform(SpawnLocation);

	AActor* SouvenirActor = GetWorld()->SpawnActor<ASouvenirActor>(SouvenirClass, SpawnTransform );
	if (SouvenirActor)
	{
		SouvenirActor->Tags.Add(FName("Souvenir"));
		Destroy(); //얻으면 삭제
	}
	
}

void ANPC_LeeSeo::RemoveBPBoxCollision()
{
	// 현재 월드에서 BP_BoxCollision 태그나 이름을 가진 모든 액터 찾기
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("BP_BoxCollision"), FoundActors);
    
	// 만약 태그로 찾지 못했다면 이름으로 시도
	if (FoundActors.Num() == 0)
	{
		TArray<AActor*> AllActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);
        
		for (AActor* Actor : AllActors)
		{
			if (Actor && Actor->GetName().Contains("BP_BoxCollision"))
			{
				FoundActors.Add(Actor);
			}
		}
	}
    
	// 찾은 모든 액터 제거
	for (AActor* Actor : FoundActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
	
}


void ANPC_LeeSeo::ResultEvent(int32 result)
{

	if(1 == State)
	{
		if(0 == result)
		{

			ChangeCleared();
			
			//귀를 막고 외친다. "너는 거짓말쟁이야" => 유품 획득
			//소녀가 비명을 지른다. -> 점프스퀘어 발동 후 사라짐 그림앞에 유품 스폰
			UGameplayStatics::PlaySound2D(GetWorld(), ScreamSound); //소리 지르기

			TestPC->StartEndNPCDialougue(false); //결과 메세지 생성
			
			//그와 동시에 불빛이 흔들림 => 길게 재생
			// 1) 조명이벤트-> 깜빡이다가 쭉 어두워지게
			if (LightControlReference)
			{
				// 순차적으로 불이 꺼지는 효과
				LightControlReference->StartLightOffSequence(1.0f);
				
				// 모든 불이 꺼진 후 2초 후에 갑자기 다시 켜짐
				//LightControlReference->StartSuddenLightOnSequence(2.0f);

				//천천히 돌면서 달림
				if(Anim)
				{
					Anim->PlayJumpSkareMontage1();
				}

				FTimerHandle JumpSquareTimerHandle1;
				GetWorldTimerManager().SetTimer(JumpSquareTimerHandle1, [this]()
				{
					
					//맵에 설치되어있던 BP_BoxCollision을 찾아서 제거
					RemoveBPBoxCollision();

					// Yaw(Z축)에 20도 더하기
					AddActorLocalRotation(FRotator(0.0f, 20.0f, 0.0f));

					// 지속적인 이동 시작 (2초 동안)
					StartMovingForward(2.0f, 1.0f);
					
					if(Anim)
					{
						Anim->PlayJumpSkareMontage2();
						UE_LOG(LogTemp, Warning, TEXT("Playing jump scare montage2"));
					}
					
					
					FTimerHandle FTimerLee;
					GetWorldTimerManager().SetTimer(FTimerLee, [this]()
					{
						
						// 모든 불을 즉시 끄기
						LightControlReference->TurnOffAllLightsImmediately();

						// 갑자기 모든 불을 켜기 (지정된 시간 후에)
						LightControlReference->StartSuddenLightOnSequence(1.0f); // 1초 후에 모든 불이 켜짐

						FTimerHandle TimerLee;
						GetWorldTimerManager().SetTimer(TimerLee, [this]()
						{
							//메쉬 아예 사라짐
							GetMesh()->SetHiddenInGame(false);
							
							// 아이템 스폰 로그
							SpawnItem(); //아이템 스폰
						}, 1.0f, false);
						
					},4.0f, false);
					
				}, 3.0f, false);
				
			}
			
		}
		else if(1 == result)
		{
			UE_LOG(LogTemp, Warning, TEXT("Processing result 1"));
			//1-2. 그림을 찢어선 안돼 => 낄낄 웃는 소리 재생. => 사망
			UGameplayStatics::PlaySound2D(GetWorld(), LSJump);

			TestPC->StartEndNPCDialougue(false); //결과 메세지 생성

			ChangeCleared();
			
			//전구가 깜빡거림

			//깜빡거림과 동시에 이서가 돌아봄
			if(Anim)
			{
				Anim->PlayJumpSkareMontage1();
			}

			//모든 타이머를 순차적으로 실행하는 함수 구현
			ExecuteJumpScareSequence();
			
		}
		else if(2 == result)
		{
			UE_LOG(LogTemp, Warning, TEXT("Processing result 2"));
			//소녀를 달래본다. "괜찮으십니까?" => 사망

			
		}
		else if(3 == result)
		{
			UE_LOG(LogTemp, Warning, TEXT("Processing result 3"));
			//도망친다 여기로 변경

			//1-3. 도망친다 => 맵에서 갑자기 소름 돋는 노래 재생.
			UGameplayStatics::PlaySound2D(GetWorld(), LSJump);
			
			//전구가 깜빡거림

			//이서 1초 사라졌다가 달려오는 점프 스케어 발동
			FTimerHandle TimeHandle1;
			GetWorldTimerManager().SetTimer(TimeHandle1, [this]()
			{
				//일시적으로 안보이게 하기
				GetMesh()->SetHiddenInGame(false);
				GetComponentByClass<UCapsuleComponent>()->SetCollisionProfileName(FName("ClearedNPC"));

				// 2초 후에 다음 단계 실행
				FTimerHandle JumpSquareTimerHandle3;
				GetWorldTimerManager().SetTimer(JumpSquareTimerHandle3, [this]()
				{
					GetMesh()->SetHiddenInGame(true);
					
					//앞으로 달리는 애니메이션 재생 : 칼 등의 요소 들고 달리기
					if(Anim)
					{
						Anim->PlayJumpSkareMontage1();
					}
					
					//점프 스케어 카메라 전환
					SwitchToMonsterCamera();
					
				}, 2.0f, false);
			}, 2.0f, false);
		}
	}
}
