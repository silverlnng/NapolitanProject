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
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/Interact/ItemActor.h"
#include "NapolitanProject/Interact/PieceActor.h"
#include "NapolitanProject/Interact/Sculpture.h"
#include "NapolitanProject/Interact/SouvenirActor.h"

ATestCharacter::ATestCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	//Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	//Mesh1P->SetOnlyOwnerSee(true);
	
	//Mesh1P->SetupAttachment(GetCapsuleComponent()); // 변경
	
	//Mesh1P->bCastDynamicShadow = false;
	//Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	//Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	GetMesh()->bCastDynamicShadow = false;
	GetMesh()->CastShadow = false;
	
	// Create a CameraComponent	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetMesh(),"head");
	CameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	CameraComponent->bUsePawnControlRotation = true;
	//CameraComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("HeadSocket"));

	PrimaryActorTick.bCanEverTick = true;


	StandingCapsuleHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	CrouchingCapsuleHalfHeight = StandingCapsuleHalfHeight / 4.0f;  // Example value, adjust as needed

	StandingCapsuleRadius =GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	CrouchingCapsuleRadius = StandingCapsuleRadius / 4.0f;
	
	StandingWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	CrouchingWalkSpeed = StandingWalkSpeed / 2.0f;  // Example value, adjust as needed

	// Ensure crouch isn't blocking movement
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	// 스프링암을 생성해서 루트에 붙이고싶다.
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocation(FVector(0 , 40 , 80));
	SpringArmComp->TargetArmLength = -200;

	// 카메라를 생성해서 스프링암에 붙이고싶다.
	ChageCameracomp = CreateDefaultSubobject<UCameraComponent>(TEXT("ChageCameracomp"));
	ChageCameracomp->SetupAttachment(SpringArmComp);
	ChageCameracomp->SetRelativeLocation(FVector(-190.f, -40.f, 0.f)); // Position the camera
	
	//ChageCameracomp->SetRelativeLocation(FVector(0 , -40 , -20));
	//ChageCameracomp->SetRelativeRotation(FRotator(0, 180, 0));

	myArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("myArrowComp"));
	myArrowComp->SetupAttachment(CameraComponent);

	// 아이템 부착할 위치 잡는 용도 
	ItemArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ItemArrowComp"));
	ItemArrowComp->SetupAttachment(CameraComponent);

}

void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();
	runSpeed = StandingWalkSpeed*3.f;
	PC = Cast<APlayerController>(Controller);
	TestPC = Cast<ATestPlayerController>(PC);
	PlayerHUD=PC->GetHUD<APlayerHUD>();

	// 타이머로 trace 작동시키기
	FTimerHandle TimerHandle;	

	GetWorldTimerManager().SetTimer(TimerHandle,this,&ATestCharacter::SphereTraceFromCamera,0.2f,true);
	

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this,&ATestCharacter::OnCapsuleOverlap);

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
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATestCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATestCharacter::Look);

		EnhancedInputComponent->BindAction(IA_Crouched, ETriggerEvent::Started, this, &ATestCharacter::CrouchToggle);

		EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Started, this, &ATestCharacter::OnRunAction);
		EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Completed, this, &ATestCharacter::EndRunAction);


		EnhancedInputComponent->BindAction(IA_Tab, ETriggerEvent::Started, this, &ATestCharacter::NoteUIToggle);

		EnhancedInputComponent->BindAction(IA_Interact, ETriggerEvent::Started, this, &ATestCharacter::OnInteraction);
		
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
	}
}

void ATestCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

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
	GetCharacterMovement()->MaxWalkSpeed=StandingWalkSpeed;
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
			GetCharacterMovement()->MaxWalkSpeed = StandingWalkSpeed;
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

void ATestCharacter::StartCrouch()
{
	GetCharacterMovement()->MaxWalkSpeed = CrouchingWalkSpeed;

	GetCapsuleComponent()->SetCapsuleRadius(CrouchingCapsuleRadius);
	GetCapsuleComponent()->SetCapsuleHalfHeight(CrouchingCapsuleHalfHeight);

	// Crouch down using Character's built-in function
	Crouch();
}

void ATestCharacter::StopCrouch()
{
	GetCharacterMovement()->MaxWalkSpeed = StandingWalkSpeed;
	GetCapsuleComponent()->SetCapsuleRadius(StandingCapsuleRadius);
	GetCapsuleComponent()->SetCapsuleHalfHeight(StandingCapsuleHalfHeight);
	// Stand up using Character's built-in function
	UnCrouch();
}

void ATestCharacter::CrouchToggle(const FInputActionValue& Value)
{
	bCrouched = !bCrouched;
	if ( bCrouched )
	{
		StartCrouch();
	}
	else
	{
		StopCrouch();
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
		break;
	case EPlayerState::Idle:
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
			break;	
	default:
		break;
	}
	
}

void ATestCharacter::NoteUIToggle(const FInputActionValue& Value)
{
	if (PlayerHUD->NoteUI->IsVisible())
	{
		PlayerHUD->NoteUI->SetVisibility(ESlateVisibility::Hidden);
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
	FColor TraceColor = InteractHit ? FColor::Red : FColor::Green;
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, TraceColor, false, 2.0f, 0, 2.0f);
	//DrawDebugSphere(GetWorld(), TraceStart, SphereRadius, 12, TraceColor, false, 2.0f);
	
	if (InteractHit && curState==EPlayerState::Idle) //  이중조건 && interact 중이 아닐때로 만들기  
	{
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, SphereRadius, 12, TraceColor, false, 2.0f);

		Interact =HitResult.GetActor();
		PlayerHUD->InteractUI->SetVisibleHBox(true);
		//PlayerHUD->InteractUI->SetVisibility(ESlateVisibility::Visible);
	}
	else if (!InteractHit && curState==EPlayerState::Idle)
	{
		//DrawDebugSphere(GetWorld(), TraceEnd, SphereRadius, 12, TraceColor, false, 2.0f);
		PlayerHUD->InteractUI->SetVisibleHBox(false);
		PlayerHUD->InteractUI->SetVisibleCrossHair(true);
		//PlayerHUD->InteractUI->SetVisibility(ESlateVisibility::Hidden);
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
	if (InteractHit && Interact)
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
		}

		AControllableLightActor* ControllableLight =Cast<AControllableLightActor>(Interact);
		
		// 라이트라면 라이트로 캐스트해서 
		//현재 조작할 라이트가 있고 그 라이트의 범위 안일떄만 작동
		if (ControllableLight)
		{
			curControllableLight=ControllableLight;
			//IsLightRangeIn=true;
			// curControllableLight 의 불키는 함수 작동시키기
			curControllableLight->TurnOnLight(true);
		}
		// 그냥 아이템 이라면 아이템으로 캐스트해서 

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
		
		ASculpture* Sculpture =Cast<ASculpture>(Interact);
		APieceActor* CurPiece =Cast<APieceActor>(curItem);
		if (CurPiece&&Sculpture)
		{
			// Sculpture 에서 내려두기 UI 출력
			Sculpture->PutDownPiece(CurPiece);
			curItem=nullptr;
		}
		
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

void ATestCharacter::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 라이트 스위치 액터 등 가까이 있는 액터와 반응 할때 만든 함수
	/*AControllableLightActor* ControllableLight = Cast<AControllableLightActor>(OtherActor);
	if (ControllableLight)
	{
		curControllableLight=ControllableLight;
		IsLightRangeIn=true;
	}*/
	
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

void ATestCharacter::PlayDamagedAnimMontage()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
	}
	
	// 이동 입력 을 못하게하고
	//bIsBeingAttacked=true;
	
	if (DamagedSecurityAnim)
	{
		PlayAnimMontage(DamagedSecurityAnim);
	}
}


