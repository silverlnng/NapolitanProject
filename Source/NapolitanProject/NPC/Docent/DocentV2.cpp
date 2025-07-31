// Fill out your copyright notice in the Description page of Project Settings.


#include "DocentV2.h"

#include "AIController.h"
#include "EngineUtils.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/Overlay.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/SaveGISubsystem.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/Interact/InteractWidget.h"
#include "NapolitanProject/Interact/Souvenir/Souvenir_Docent.h"
#include "NapolitanProject/YJ/DeadEndingWidget.h"
#include "NapolitanProject/YJ/SoundControlActor.h"

// Sets default values
ADocentV2::ADocentV2()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AudioComp =CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(GetCapsuleComponent());
	// 몬스터 카메라 생성 및 초기화
	MonsterCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MonsterCamera"));
	MonsterCamera->SetupAttachment(GetMesh(),FName(TEXT("HeadSocket"))); // 루트에 부착
	MonsterCamera->bUsePawnControlRotation = false; // 플레이어 조작 방지

	//콜린전 채널
	SpotLightComp=CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComponent"));
	SpotLightComp->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void ADocentV2::BeginPlay()
{
	Super::BeginPlay();

	TestPC = GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	
	if (TestPC)
	{
		MainCharacter =TestPC->GetPawn<ATestCharacter>();
	}
	
	PlayerHUD =TestPC->GetHUD<APlayerHUD>();
	
	AIController = Cast<AAIController>(GetController());

	for (TActorIterator<ASoundControlActor> It(GetWorld(), ASoundControlActor::StaticClass()); It; ++It)
	{
		SoundControlActor = *It;
	}
	//StartTurnDetect();
	SpotLightComp->SetIntensity(0.f);
}

// Called every frame
void ADocentV2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//DrawDebugSphere(GetWorld(), GetActorLocation(), MaxDetectionDistance, 12, FColor::Blue, false, 0.1f);

	//DrawDebugSphere(GetWorld(), GetActorLocation(), AttackRange, 12, FColor::Red, false, 0.1f);

	if (MainCharacter)
	{
		FVector CurrentLocation = MainCharacter->GetActorLocation();
	
		float DistanceToPlayer = FVector::Dist(GetActorLocation(), CurrentLocation);
		
		if (!InMaxDetectionDistance && DistanceToPlayer <= MaxDetectionDistance)
		{
			InMaxDetectionDistance=true;
			// 음원변경
			if (SoundControlActor&&SoundControlActor->DocentBG)
			{
				SoundControlActor->BGSoundChange(SoundControlActor->DocentBG);
			}

			// 처음 진입 딱한번 만 .
			if (!bOnlyOnce)
			{
				// bOnlyOnce=true; //=> 이걸 마지막대사 끝날때 
				// 대사나오도록 하기
				Interact();
				// 대사 끝나고 StartTurnDetect 시작하도록 하기
				// 그리고 마지막 대사할때 알아서 닫도록 하기 
			}
			else
			{
				// 이때부터 StartTurnDetect(); 시작함
				StartTurnDetect();
			}
			
			
		}
		if (InMaxDetectionDistance&&DistanceToPlayer>MaxDetectionDistance)
		{
			InMaxDetectionDistance=false;
			// 음원변경
			if (SoundControlActor&&SoundControlActor->LobbyRoom)
			{
				SoundControlActor->BGSoundChange(SoundControlActor->LobbyRoom);
			}
			StopAllTurnDetect();
		}
	}
	
	if (bCanDetectMovement && MainCharacter)
	{
		DetectPlayerMovement();
	}
	
	// 추적 상태일 때만 체크
	if (bHasDetectedPlayer && MainCharacter)
	{
		float Distance = FVector::Dist(GetActorLocation(), MainCharacter->GetActorLocation());

		if (Distance <= AttackRange)
		{
			UE_LOG(LogTemp, Warning, TEXT("💥 공격 범위 도달!"));

			// 공격 애니메이션 실행
			PlayAttackAnimation();

			// 이후 계속 공격하지 않도록 추적 상태 종료
			bHasDetectedPlayer = false;

			// AI 이동도 멈춤
			if (AIController)
			{
				AIController->StopMovement();
			}
		}
	}
}




// ChoseRandomTimeTurnRightAnim 시작자체를
void ADocentV2::StartTurnDetect()
{
	float RandValue = FMath::FRand();
	float ChosenDelay = 0.f;

	if (RandValue <= 0.6f)
	{
		ChosenDelay=FMath::FRandRange(2.0f,2.6f);
		//ChosenDelay = FMath::RandBool() ? 2.0f : 2.6f;
	}
	else
	{
		ChosenDelay = 3.0f;
	}

	UE_LOG(LogTemp, Log, TEXT("뒤 돌아볼때까지 딜레이: %.1f초"), ChosenDelay);


	
	GetWorldTimerManager().SetTimer(
			  PlayTurnRightAnim,
			  this,
			  &ADocentV2::PlayTurnRightAnimation,
			  ChosenDelay,
			  false
		  );
	
	
}

void ADocentV2::PlayTurnRightAnimation()
{
	
	GetMesh()->PlayAnimation(TurnAroundMontage,false);
	
	if (Souvenir_Docent)
	{
		Souvenir_Docent->ChangeCollResponseIgnore();
		UE_LOG(LogTemp, Log, TEXT("Souvenir_Docent 콜리전 수정"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Souvenir_Docent 없음"));
	}
	
	float RandValue = FMath::FRand();
	float ChosenDelay = 0.f;

	if (RandValue <= 0.6f)
	{
		ChosenDelay = 8.0f;
	}
	else
	{
		ChosenDelay = 5.0f;
	}
	
	UE_LOG(LogTemp, Log, TEXT("앞으로 돌아볼때까지 딜레이: %.1f초"), ChosenDelay);
	
	// 0.5초 지난후 부터 플레이어의 움직임을 감지
	GetWorldTimerManager().SetTimer(
		  StartDetectionTimerHandle,
		  this,
		  &ADocentV2::StartMovementDetection,
		  1.0f,
		  false
	  );
	
	// 그리고 다시 원래대로 회전도 해야함 + 플레이어 감지 스탑

	// 배경음도 잠시 멈추기
	
	GetWorld()->GetTimerManager().SetTimer(StopDetectionTimerHandle,[this]()
	{
		PlayTurnOriginAnimation();
		StopMovementDetection();
		// 배경음 다시 플레이되도록 하기 
	},ChosenDelay,false);
}

void ADocentV2::PlayTurnOriginAnimation()
{
	if (TurnOriginMontage)
	{
		GetMesh()->PlayAnimation(TurnOriginMontage,false);
	}

	if (Souvenir_Docent)
	{
		Souvenir_Docent->ChangeCollResponseBlock();
	}
	
	// 다시 시작
	StartTurnDetect();
}

void ADocentV2::StartMovementDetection()
{
	bCanDetectMovement = true;

	// 현재 위치 저장
	if (MainCharacter)
	{
		LastPlayerLocation = MainCharacter->GetActorLocation();
	}
	
	UE_LOG(LogTemp, Log, TEXT("🚨 감지 시작!"));
}


void ADocentV2::DetectPlayerMovement()
{
	FVector CurrentLocation = MainCharacter->GetActorLocation();

	float DistanceToPlayer = FVector::Dist(GetActorLocation(), CurrentLocation);
	
	if (DistanceToPlayer > MaxDetectionDistance)
	{
		UE_LOG(LogTemp, Verbose, TEXT("❌ 감지 거리 초과: %.1fcm"), DistanceToPlayer);
		return; // 너무 멀면 감지하지 않음
	}
	
	// 위치 변화량 감지
	float MovementDelta = FVector::Dist(LastPlayerLocation, CurrentLocation);

	if (MovementDelta > DetectRange) // 임계값 조정 가능
	{
		UE_LOG(LogTemp, Warning, TEXT(" 플레이어 움직임 감지됨!"));

		// 감지되면 실행할 함수
		// 1. 감지 중지
		bCanDetectMovement = false;
		bHasDetectedPlayer = true;
		// 2. 타이머 취소
		if (GetWorldTimerManager().IsTimerActive(StopDetectionTimerHandle))
		{
			GetWorldTimerManager().ClearTimer(StopDetectionTimerHandle);
			UE_LOG(LogTemp, Log, TEXT("🛑 StopDetection 타이머 취소됨"));
		}

		// 배경음은 중지하고 , 감지 소리 작동시키기  
		SoundControlActor->AudioComp2->Stop();
		
		if (DetectSound)
		{
			UGameplayStatics::PlaySound2D(this, DetectSound);
		}

		// 탐지했을때 애니메이션 먼저실행
		if (DetectMontage)
		{
			GetMesh()->PlayAnimation(DetectMontage,false);
		}

		// 달리기+ 추적을 살짝만 지연시키기
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			if (AIController && MainCharacter)
			{
				AIController->MoveToActor(MainCharacter);
				// 달리는 애니메이션 실행
				if (RunMontage)
				{
					GetMesh()->PlayAnimation(RunMontage,true);
				}
				UE_LOG(LogTemp, Log, TEXT("🏃 몬스터가 플레이어를 추적 시작"));
			}
		}, 1.0f, false);
		
		

		// 이후 반복 감지 막기 위해 종료
		return;
	}
	LastPlayerLocation = CurrentLocation;
}

void ADocentV2::StopMovementDetection()
{
	bCanDetectMovement = false;
	UE_LOG(LogTemp, Log, TEXT("🛑 감지 종료"));
}

void ADocentV2::PlayAttackAnimation()
{
	if (AttackMontage && !IsPlayingRootMotion()) // 중복 재생 방지
	{
		PlayAnimMontage(AttackMontage);
		UE_LOG(LogTemp, Log, TEXT("👊 공격 애니메이션 실행됨"));
	}

	MainCharacter->SetActorHiddenInGame(true);
	// 사망이벤트 만 발생시킴
	MainCharacter->bIsBeingAttacked=true;
	//카메라 쉐이크 .
	SwitchToMonsterCamera();
	
	if (AttackSound)
	{
		UGameplayStatics::PlaySound2D(this, AttackSound);
	}
	

	FTimerHandle UITimer2;
	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this]()
	{
		if (PlayerHUD )
		{
		
			PlayerHUD->PlayDeadVignetteEffect();
		}
	},2.5f,false);
	
	//시간지연 주고 사망 UI 나오도록 
	FTimerHandle UITimer;
	GetWorld()->GetTimerManager().SetTimer(UITimer,[this]()
	{
		MainCharacter->SetPlayerState(EPlayerState::UI);
		
		if (PlayerHUD &&PlayerHUD->DeadEndingWidgetUI)
		{
			PlayerHUD->DeadEndingWidgetUI->SetVisibility(ESlateVisibility::Visible);
			PlayerHUD->DeadEndingWidgetUI->Overlay_Docent->SetVisibility(ESlateVisibility::Visible);		
			PlayerHUD->DeadEndingWidgetUI->SetTextBlock_description(description);
		}
	},3.5f,false);
}

void ADocentV2::PickUPNote()
{

	// 모든거 멈추도록 하기
	StopAllTurnDetect();

	// 도슨트의 카메라는 제거하기
	this->CameraComp->SetActive(false);
	this->MonsterCamera->SetActive(false);
	
	State=2;
	// 도슨트가 그림속을 향해 사라지도록 만들기
	// 무조건 그림앞을 향해 보고
	/*UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		UE_LOG(LogTemp, Log, TEXT("🛑 도슨트의 AnimInstance"));

		//모든 애니메이션 실행 멈추고 . 그림속으로 들어가도록 하고싶다
		
		bool bIsTurnBack=AnimInstance->Montage_IsPlaying(TurnAroundMontage);
		if (bIsTurnBack)
		{
			UE_LOG(LogTemp, Log, TEXT("🛑TurnAround"));
			if (TurnOriginMontage)
			{
				GetMesh()->PlayAnimation(TurnOriginMontage,false);
			}
			// 시간지연을 주고
			FTimerHandle Timer1;
			GetWorld()->GetTimerManager().SetTimer(Timer1,[this]()
			{
				if (GoingUpMontage)
				{
					GetMesh()->PlayAnimation(GoingUpMontage,true);
				}
			},3.5f,false);
			
		}
		else
		{
			//조금 높이가 있는 곳으로 걷는 애니메이션
			// 코드로 앞으로 조금씩 이동시키기 
			if (GoingUpMontage)
			{
				GetMesh()->PlayAnimation(GoingUpMontage,true);
			}
		}
		
	}
	
	// 그리고 시간지연 주고 사라지도록 하기
	FTimerHandle Timer2;
	GetWorld()->GetTimerManager().SetTimer(Timer2,[this]()
	{
		Destroy();
		// 노래원래대로 변경하기
		if (SoundControlActor&&SoundControlActor->LobbyRoom)
		{
			SoundControlActor->BGSoundChange(SoundControlActor->LobbyRoom);
		}
	},8.0f,false);*/
}

void ADocentV2::CloseUPCam()
{
	UE_LOG(LogTemp, Log, TEXT("🛑ADocentV2::CloseUPCam"));
	if (CheckUnderMontage)
	{
		GetMesh()->PlayAnimation(CheckUnderMontage,false);
	}
	
	this->AttachToComponent(MainCharacter->CenterArrowComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	// Arrow를 바라보도록 회전 보간
	FRotator DesiredRot = (MainCharacter->GetActorLocation() - this->GetActorLocation()).Rotation();
	
	this->SetActorRotation(DesiredRot);

	// 다리 안보이도록 숨기기
}

void ADocentV2::DetachDestroy()
{
	this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	FTimerHandle Timer2;
	GetWorld()->GetTimerManager().SetTimer(Timer2,[this]()
	{
		Destroy();
		// 노래원래대로 변경하기
		if (SoundControlActor&&SoundControlActor->LobbyRoom)
		{
			SoundControlActor->BGSoundChange(SoundControlActor->LobbyRoom);
		}
	},2.0f,false);
}

void ADocentV2::DocentLightOn()
{
	SpotLightComp->SetIntensity(LightIntensity);
}



void ADocentV2::Interact()
{
	Super::Interact();
}

int32 ADocentV2::GetNPCID()
{
	return NPC_ID;
}

int32 ADocentV2::GetState()
{
	return State;
}

void ADocentV2::SwitchToMonsterCamera()
{
	DocentLightOn();
	this->CameraComp->SetActive(false);
	this->MonsterCamera->SetActive(true);
	if (TestPC && MonsterCamera)
	{
		// 카메라 전환
		TestPC->SetViewTargetWithBlend(this, 0.01f); // 0.5초 동안 부드럽게 전환
	}
}

void ADocentV2::StopAllTurnDetect()
{
	// 타이머 종료하고 
	
	if (GetWorldTimerManager().IsTimerActive(StartDetectionTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(StartDetectionTimerHandle);
		UE_LOG(LogTemp, Log, TEXT("🛑 StartDetectionTimerHandle 타이머 취소됨"));
	}
	
	if (GetWorldTimerManager().IsTimerActive(PlayTurnRightAnim))
	{
		GetWorldTimerManager().ClearTimer(PlayTurnRightAnim);
		UE_LOG(LogTemp, Log, TEXT("🛑 PlayTurnRightAnim 타이머 취소됨"));
	}

	
	StopMovementDetection();
}
