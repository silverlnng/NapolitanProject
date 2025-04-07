// Fill out your copyright notice in the Description page of Project Settings.


#include "TestCharacter.h"

#include "../Interact/ClueActor.h"
#include "../Interact/ControllableLightActor.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "../Interact/InteractWidget.h"
#include "NapolitanProject/NPC/NPCCharacter.h"
#include "PlayerHUD.h"
#include "TestPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "../YJ/NoteUI/NoteWidget.h"
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/Interact/ItemActor.h"
#include "NapolitanProject/Interact/SouvenirActor.h"
#include "NapolitanProject/Interact/TargetForItem.h"
#include "NapolitanProject/YJ/CheckPoint.h"
#include "NapolitanProject/YJ/DeadEndingWidget.h"
#include "NapolitanProject/YJ/ESCWidget.h"
#include "NapolitanProject/YJ/DialogueUI/NPCDialogueWidget.h"
#include "NapolitanProject/YJ/DialogueUI/NPCResultWidget.h"
#include "NapolitanProject/YJ/NoteUI/InventoryWidget.h"
#include "Perception/AISense_Hearing.h"


ATestCharacter::ATestCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	GetMesh()->bCastDynamicShadow = false;
	GetMesh()->CastShadow = false;
	
	// Create a CameraComponent	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetMesh(),"head");
	CameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	CameraComponent->bUsePawnControlRotation = true;
	//CameraComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("HeadSocket"));

	PrimaryActorTick.bCanEverTick = true;
	
	// 스프링암을 생성해서 루트에 붙이고싶다.
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocation(FVector(0 , 40 , 80));
	SpringArmComp->TargetArmLength = -200;

	// 카메라를 생성해서 스프링암에 붙이고싶다.
	ChageCameracomp = CreateDefaultSubobject<UCameraComponent>(TEXT("ChageCameracomp"));
	ChageCameracomp->SetupAttachment(SpringArmComp);
	ChageCameracomp->SetRelativeLocation(FVector(-190.f, -40.f, 0.f)); // Position the camera
	
	leftArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("myArrowComp"));
	leftArrowComp->SetupAttachment(CameraComponent);

	// 아이템 부착할 위치 잡는 용도 
	ItemArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ItemArrowComp"));
	ItemArrowComp->SetupAttachment(CameraComponent);

	AudioComp =CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(GetCapsuleComponent());

	CenterArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("CenterArrowComp"));
	CenterArrowComp->SetupAttachment(CameraComponent);
	
}

void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();
	DefaultWalkSpeed=GetCharacterMovement()->MaxWalkSpeed;
	runSpeed = GetCharacterMovement()->MaxWalkSpeed*3.f;
	PC = Cast<APlayerController>(Controller);
	TestPC = Cast<ATestPlayerController>(PC);
	PlayerHUD=PC->GetHUD<APlayerHUD>();

	// 타이머로 trace 작동시키기
	FTimerHandle TimerHandle;	

	GetWorldTimerManager().SetTimer(TimerHandle,this,&ATestCharacter::SphereTraceFromCamera,0.2f,true);
	
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this,&ATestCharacter::EndCapsuleOverlap);
	
	
	
}

void ATestCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bIsRunning) // 달리기 누르면 실행하는 함수
	{
		UpdateRunAction(DeltaSeconds);
	}
	else
	{
		UpdateNotRunAction(DeltaSeconds);
	}
	// GetCapsuleComponent()->SetCapsuleHalfHeight(FMath::Lerp());

	
	//1-3. 김영수 위대한 빨간 등대 이벤트
	if (bIsCameraTransitioning && ChageCameracomp)
	{
		//카메라 위치와 각도를 부드럽게 보간
		FVector CurrentLocation = ChageCameracomp->GetRelativeLocation();
		FRotator CurrentRotation = ChageCameracomp->GetRelativeRotation();

		FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetCameraLocation, DeltaSeconds, CameraTransitionSpeed);
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetCameraRotation, DeltaSeconds, CameraTransitionSpeed);

		ChageCameracomp->SetRelativeLocation(NewLocation);
		ChageCameracomp->SetRelativeRotation(NewRotation);

		// FieldOfView 값을 부드럽게 보간
		CurrentFieldOfView = FMath::FInterpTo(CurrentFieldOfView, TargetFieldOfView, DeltaSeconds, CameraTransitionSpeed);
		ChageCameracomp->SetFieldOfView(CurrentFieldOfView);

		// 목표 위치와 각도에 도달하면 전환 종료
		if (NewLocation.Equals(TargetCameraLocation, 0.1f) && NewRotation.Equals(TargetCameraRotation, 0.1f))
		{
			bIsCameraTransitioning = false;
			bIsFieldOfViewTransitioning = true; // FieldOfView 전환 시작
		}
	}

	//줌인지 확인
	if (bIsFieldOfViewTransitioning && ChageCameracomp && bIsCameraTransitioning == false)
	{
		//보간
		CurrentFieldOfView = FMath::FInterpTo(ChageCameracomp->FieldOfView, 37.0f, DeltaSeconds, CameraTransitionSpeed);
		ChageCameracomp->SetFieldOfView(CurrentFieldOfView);

		// 목표 FieldOfView에 도달하면 전환 종료
		if (FMath::IsNearlyEqual(CurrentFieldOfView, 37.0f, 0.1f))
		{
			bIsFieldOfViewTransitioning = false;
		}
	}
	
}


void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	auto* pc = Cast<APlayerController>(Controller);

	if (pc)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer()))
		{
			// add the mapping context so we get controls
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
	
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ATestCharacter::MyJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ATestCharacter::MyJumpCompeleted);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATestCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATestCharacter::Look);


		EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Started, this, &ATestCharacter::OnRunAction);
		EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Completed, this, &ATestCharacter::EndRunAction);

		EnhancedInputComponent->BindAction(IA_Inventory, ETriggerEvent::Started, this, &ATestCharacter::InventoryUIToggle);

		EnhancedInputComponent->BindAction(IA_NoteUI, ETriggerEvent::Started, this, &ATestCharacter::NoteUIToggle);

		EnhancedInputComponent->BindAction(IA_Interact, ETriggerEvent::Started, this, &ATestCharacter::OnInteraction);

		EnhancedInputComponent->BindAction(IA_ESC, ETriggerEvent::Started, this, &ATestCharacter::ESCUIToggle);

		EnhancedInputComponent->BindAction(IA_P, ETriggerEvent::Started, this, &ATestCharacter::ESCUIToggle);
		
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}



void ATestCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (!bIsBeingAttacked && Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
		OnEnablePlayerNoise.Broadcast();
	}
}
void ATestCharacter::MyJump()
{

	if (curState==EPlayerState::Talking)
	{
		// 대화창 ui 에서 다음, 이전 으로 넘어가도록 하기
		if (PlayerHUD)
		{
			if (PlayerHUD->NPCDialogueUI->GetVisibility()==ESlateVisibility::Visible)
			{
				if (PlayerHUD->NPCDialogueUI->Btn_Next->GetVisibility()==ESlateVisibility::Visible)
				{
					
					PlayerHUD->NPCDialogueUI->OnClickfrontButton();
				}
				
			}
			else if (PlayerHUD->NPCResultUI->IsVisible())
			{
				if (PlayerHUD->NPCResultUI->Btn_Next->GetVisibility()==ESlateVisibility::Visible)
				{
					PlayerHUD->NPCResultUI->OnClickfrontButton();
				}
			}
		
		}
	}
	else
	{
		bPressedJump = true;
		JumpKeyHoldTime = 0.0f;
		OnEnablePlayerNoise.Broadcast();
	}
	
}

void ATestCharacter::MyJumpCompeleted()
{
	if (curState==EPlayerState::Talking){return;}
	bPressedJump = false;
	ResetJumpState();
}

void ATestCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>()*LookSpeed;

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATestCharacter::OnRunAction(const FInputActionValue& Value)
{
	// 누르면 함수 호출
	bIsRunning=true;
	if (bIsRunGageRemains)
	{
		GetCharacterMovement()->MaxWalkSpeed=runSpeed;
	}
}

void ATestCharacter::EndRunAction(const FInputActionValue& Value)
{
	// 타이머 종료
	bIsRunning=false;
	GetCharacterMovement()->MaxWalkSpeed=DefaultWalkSpeed;
}

void ATestCharacter::UpdateRunAction(float DeltaTime)
{
		// 누르는 동안 계속 호
	
		RunGage -= DeltaTime;
		RunGage=FMath::Max(RunGage,0.f);
		// 5초가 넘으면 원래 속도로 전환
		if (RunGage <= 0)
		{
			bIsRunGageRemains=false;
			GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
		}
	
}
void ATestCharacter::UpdateNotRunAction(float DeltaTime)
{
	// 누르는 동안 계속 호출
	RunGage += DeltaTime;
	RunGage = FMath::Min(runCooltime, RunGage);
	// 5초가 넘으면 원래 속도로 전환
	if (RunGage >= runCooltime)
	{
		bIsRunGageRemains=true;
	}
}



void ATestCharacter::SetPlayerState(EPlayerState newState)
{
	curState=newState;

	switch (curState)
	{
	case EPlayerState::UI:
		// 노트  ui 볼때
		PC->SetInputMode(FInputModeGameAndUI());
		PC->SetShowMouseCursor(true);
		bIsBeingAttacked=true;
		break;
	case EPlayerState::Idle:
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
		bIsBeingAttacked=false;
			break;
	case EPlayerState::Talking:
		PC->SetInputMode(FInputModeGameAndUI());
		PC->SetShowMouseCursor(true);
		bIsBeingAttacked=true;
		break;
	default:
		break;
	}
}

void ATestCharacter::ESCUIToggle(const FInputActionValue& Value)
{
	if (PlayerHUD->UESC_UI->IsVisible()) // 노트가 보이는 중 이면
	{
		PlayerHUD->UESC_UI->SetVisibility(ESlateVisibility::Hidden); // Inventory를 닫아라
		
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
		if (NoteUICloseSound)
		{
			UGameplayStatics::PlaySound2D(this, NoteUICloseSound);
		}
	}
	else
	{
		UWidgetBlueprintLibrary::SetFocusToGameViewport();
		PlayerHUD->UESC_UI->SetVisibility(ESlateVisibility::Visible);
		PC->SetInputMode(FInputModeGameAndUI());
		PC->SetShowMouseCursor(true);
		if (NoteUIOpenSound)
		{
			UGameplayStatics::PlaySound2D(this, NoteUIOpenSound);
		}
	}
}

void ATestCharacter::InventoryUIToggle(const FInputActionValue& Value)
{
	if (!b_IA_Inven_Allowed){return;}
	if (curState==EPlayerState::Talking){return;}
	
	if (PlayerHUD->InventoryUI->IsVisible()) // 노트가 보이는 중 이면
	{
		PlayerHUD->InventoryUI->SetVisibility(ESlateVisibility::Hidden); // Inventory를 닫아라

		//Inventory 초기화 작업
		PlayerHUD->InventoryUI->WhenClosed();
		
		if (PlayerHUD->InteractUI->CanvasPanel_Clue->GetVisibility() == ESlateVisibility::Visible)
		{
			return;
		}
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
		SetPlayerState(EPlayerState::Idle);
		if (NoteUICloseSound)
		{
			UGameplayStatics::PlaySound2D(this, NoteUICloseSound);
		}
	}
	else
	{
		UWidgetBlueprintLibrary::SetFocusToGameViewport();
		PlayerHUD->InventoryUI->SetVisibility(ESlateVisibility::Visible);
		PC->SetInputMode(FInputModeGameAndUI());
		PC->SetShowMouseCursor(true);
		SetPlayerState(EPlayerState::UI);
		//
		if (NoteUIOpenSound)
		{
			UGameplayStatics::PlaySound2D(this, NoteUIOpenSound);
		}
	}
}

void ATestCharacter::NoteUIToggle(const FInputActionValue& Value)
{

	if (!b_IA_Note_Allowed){return;}
	if (curState==EPlayerState::Talking){return;}
	
	if (PlayerHUD->NoteUI->IsVisible()) // 노트가 보이는 중 이면
	{
		PlayerHUD->NoteUI->SetVisibility(ESlateVisibility::Hidden); // 노트를 닫아라
		
		if (PlayerHUD->InteractUI->CanvasPanel_Clue->GetVisibility() == ESlateVisibility::Visible)
		{
			return;
		}
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
		SetPlayerState(EPlayerState::Idle);
		if (NoteUICloseSound)
		{
			UGameplayStatics::PlaySound2D(this, NoteUICloseSound);
		}
	}
	else
	{
		UWidgetBlueprintLibrary::SetFocusToGameViewport();
		PlayerHUD->NoteUI->SetVisibility(ESlateVisibility::Visible);
		PC->SetInputMode(FInputModeGameAndUI());
		PC->SetShowMouseCursor(true);
		SetPlayerState(EPlayerState::UI);
		//
		if (NoteUIOpenSound)
		{
			UGameplayStatics::PlaySound2D(this, NoteUIOpenSound);
		}
	}
}

void ATestCharacter::SphereTraceFromCamera()
{
	// 타이머로 작동시키기

	FVector CameraLocation;
	FRotator CameraRotation;
	PC->GetPlayerViewPoint(CameraLocation,CameraRotation);

	// 트레이스 시작점 및 끝점 설정
	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + (CameraRotation.Vector() * traceLength); // 1000 유닛 앞까지 트레이스

	// 트레이스 채널 설정 (여기서는 ECC_Visibility 사용)
	// Interact : ECC_GameTraceChannel3
	ECollisionChannel TraceChannel = ECC_GameTraceChannel3;
	
	// 트레이스 파라미터 설정
	FHitResult HitResult;
	
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	//bool SweepSingleByChannel(struct FHitResult& OutHit, const FVector& Start, const FVector& End, const FQuat& Rot, ECollisionChannel TraceChannel, const FCollisionShape& CollisionShape, const FCollisionQueryParams& Params = FCollisionQueryParams::DefaultQueryParam, const FCollisionResponseParams& ResponseParam = FCollisionResponseParams::DefaultResponseParam) const;

	InteractHit = GetWorld()->SweepSingleByChannel(
		 HitResult,
		 TraceStart,
		 TraceEnd,
		 FQuat::Identity, // 회전 없음
		 TraceChannel,
		 FCollisionShape::MakeSphere(SphereRadius),
		 TraceParams // 탐색 방법에 대한 설정 값을 모아둔 구조체
	 );

	// 디버그용으로 트레이스 구체를 그립니다
	//FColor TraceColor = InteractHit ? FColor::Red : FColor::Green;
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, TraceColor, false, 2.0f, 0, 2.0f);
	//DrawDebugSphere(GetWorld(), TraceStart, SphereRadius, 12, TraceColor, false, 2.0f);
	
	if (InteractHit && curState==EPlayerState::Idle) //  이중조건 && interact 중이 아닐때로 만들기  
	{
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, SphereRadius, 12, TraceColor, false, 2.0f);
		if (HitResult.GetActor()->ActorHasTag(FName(TEXT("wall"))))
		{
			InteractHit=false;
			return;
		}
		Interact =HitResult.GetActor();
		PlayerHUD->InteractUI->SetVisibleHBox(true);
		
	}
	else if (!InteractHit && curState==EPlayerState::Idle)
	{
		//DrawDebugSphere(GetWorld(), TraceEnd, SphereRadius, 12, TraceColor, false, 2.0f);
		PlayerHUD->InteractUI->SetVisibleHBox(false);
		PlayerHUD->InteractUI->SetVisibleCrossHair(true);
		
		TestPC->curNPC=nullptr;
		curControllableLight=nullptr;
	}
	else if (curState==EPlayerState::Talking||curState==EPlayerState::UI)
	{
		PlayerHUD->InteractUI->SetVisibleHBox(false);
		PlayerHUD->InteractUI->SetVisibleCrossHair(false);
	}
}

//E키 누르면 실행되는 함수
void ATestCharacter::OnInteraction()
{
	if (InteractHit && Interact && curState!=EPlayerState::Talking && curState!=EPlayerState::UI)
	{
		// 상호작용 대상에게 만들어져있는 상호작용 함수 호출시키기
		 // Interact 을 npc로 캐스팅 가능하다면
		ANPCCharacter* InteractNPC =Cast<ANPCCharacter>(Interact);
		if (InteractNPC)
		{
		// 컨트롤러 의  curNPC에 담아주기 
			TestPC->curNPC =InteractNPC;
		   // TestPC 에서 대화창 시작하는 함수 시작하기
			
			TestPC->curNPC->playTalkAnimMontage();
			
			TestPC->StartEndNPCDialougue(true);
			TestPC->SetNPCDialougueText(0);
			// 나의 상태 변화
			SetPlayerState(EPlayerState::Talking);

			if (NPCEventSoundWave)
			{
				UGameplayStatics::PlaySound2D(this, NPCEventSoundWave);
			}
		}
		
		// 단서라면 단서로 캐스트
		// 단서를 카메라 가까이 나오고 시간 지연 주고 스크린 ui 으로 각자의 내용이 나오도록 하기
		AClueActor* Clue =Cast<AClueActor>(Interact);
		if (Clue)
		{
			Clue->LookAt();
		}

		ASouvenirActor* Souvenir=Cast<ASouvenirActor>(Interact);
		if (Souvenir)
		{
			Souvenir->OnPickup();
		}

		AItemActor* ItemActor =Cast<AItemActor>(Interact);
		
		if (!curItem && ItemActor)
		{
			curItem=ItemActor;
			ItemActor->OnPickup();
		}

		// 저장할때
		
		ACheckPoint* CheckPoint =Cast<ACheckPoint>(Interact);
		
		if (CheckPoint)
		{
			CheckPoint->VisibleSaveWidget();
		}

///////////////// 아이템을 내려놓는 대상 /////////////////////////

		//현재 아이템 curItem 을 받아서 검증 하는 방법으로
		ATargetForItem* TargetForItem =Cast<ATargetForItem>(Interact);
		if (TargetForItem&&curItem)
		{
			TargetForItem->CheckItem(curItem);
		}
		else if (TargetForItem&&!curItem)
		{
			TargetForItem->NoItem();
		}
		
///////////////// 2층에서만 사용하는 기능은 델리게이트로 사용하기.//////////////////////////
		OnSpecialInteraction.Broadcast(Interact);
		
	}
}

void ATestCharacter::AdjustCameraPosition()
{
	//영수 1-3 : 위대한 빨간 등대 선택 시 사망엔딩 -> 카메라 회전 후 UI 교수형
	if (PC)
	{
		// 전환할 카메라 컴포넌트가 유효한지 확인
		if (ChageCameracomp)
		{
			// 플레이어의 뷰를 새 카메라 컴포넌트로 업데이트
			CameraComponent->Deactivate();
			ChageCameracomp->Activate();

			//목표 위치와 각도를 설정
			TargetCameraLocation = FVector(-40.f, -40.f, 1.f);
			TargetCameraRotation = FRotator(0.f, -180.f, 0.f);

			//카메라 전환 플래그와 속도 설정
			bIsCameraTransitioning = true;
			CameraTransitionSpeed = 2.0f; //카메라 전환 속도 조절

			// 목표 FieldOfView 설정
			bIsFieldOfViewTransitioning = false;
			TargetFieldOfView = 45.0f;
			CurrentFieldOfView = ChageCameracomp->FieldOfView;
			
		}
	}
	
	
}

void ATestCharacter::HangNeckUp()
{
	//이동할 목표 위치 설정
	FVector CurrentLocation = GetMesh()->GetRelativeLocation();
	TargetNeckLocation = FVector(CurrentLocation.X, CurrentLocation.Y, CurrentLocation.Z + 50.0f);

	//타이머 시작
	GetWorldTimerManager().SetTimer(MoveMeshTimerHandle, this, &ATestCharacter::UpdateMeshLocation, 0.01f, true);
}

void ATestCharacter::UpdateMeshLocation()
{
	//현재의 위치를 계산하여 목표 위치로 점진적으로 이동
	FVector CurrentLocation = GetMesh()->GetRelativeLocation();
	FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetNeckLocation, GetWorld()->DeltaTimeSeconds, 5.0f);

	// 새로운 위치 설정
	GetMesh()->SetRelativeLocation(NewLocation);

	// 목표 위치에 도달하면 이동 중지
	if (FVector::Dist(NewLocation, TargetNeckLocation) <= 1.0f)
	{
		GetWorldTimerManager().ClearTimer(MoveMeshTimerHandle);
	}
}


void ATestCharacter::EndCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (curControllableLight)
	{
		curControllableLight=nullptr;
		IsLightRangeIn=false;
	}
}

void ATestCharacter::DamagedToSecurity()
{
	/*if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
	}*/

	// 피격 소리 
	
	// 이동 입력 을 못하게하고
	//bIsBeingAttacked=true;
	
	/*if (DamagedSecurityAnim)
	{
		PlayAnimMontage(DamagedSecurityAnim);
	}*/
	Health--;
	// 피격시 나오는 ui 나오도록 하기

	PlayerHUD->InteractUI->PlayHitAnim();

	// health 체크해서 0 이하이면 사망이벤트 나오도록 해야함 
	if (Health==0)
	{
		SetPlayerState(EPlayerState::UI);
		
		if (PlayerHUD &&PlayerHUD->DeadEndingWidgetUI)
		{
			PlayerHUD->DeadEndingWidgetUI->SetVisibility(ESlateVisibility::Visible);
			FString name= FString(TEXT("<Red_Big>검은경비원 에게</>"));
			PlayerHUD->DeadEndingWidgetUI->SetRichText_Name(name);
			PlayerHUD->DeadEndingWidgetUI->StartLerpTimer();
		}
		
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->StopMovementImmediately();
		}
		// 이동 입력 을 못하게하고
		bIsBeingAttacked=true;
		
	}
	
}

void ATestCharacter::PlayHeartSound()
{
	if (AudioComp->IsPlaying())
	{
		return;
	}
	if (HeartSound)
	{
		AudioComp->SetSound(HeartSound);
		AudioComp->Play();
	}
}

void ATestCharacter::PlaySound(USoundWave* Sound)
{
	if (AudioComp->IsPlaying())
	{
		AudioComp->Stop();
	}
	if (Sound)
	{
		AudioComp->SetSound(Sound);
		AudioComp->Play();
	}
}

void ATestCharacter::StopSound()
{
	if (AudioComp->IsPlaying())
	{
		AudioComp->Stop();
	}
}
