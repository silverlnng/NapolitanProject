// Fill out your copyright notice in the Description page of Project Settings.


#include "TestCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerHUD.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NoteUI/NoteWidget.h"

ATestCharacter::ATestCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	PrimaryActorTick.bCanEverTick = true;


	StandingCapsuleHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	CrouchingCapsuleHalfHeight = StandingCapsuleHalfHeight / 4.0f;  // Example value, adjust as needed

	StandingCapsuleRadius =GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	CrouchingCapsuleRadius = StandingCapsuleRadius / 4.0f;
	
	StandingWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	CrouchingWalkSpeed = StandingWalkSpeed / 2.0f;  // Example value, adjust as needed

	// Ensure crouch isn't blocking movement
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
}

void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();
	runSpeed = StandingWalkSpeed*3.f;
	PC = Cast<APlayerController>(Controller);
	PlayerHUD=PC->GetHUD<APlayerHUD>();

	// 타이머로 trace 작동시키기
	
	
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

	if (Controller != nullptr)
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

void ATestCharacter::NoteUIToggle(const FInputActionValue& Value)
{
	if (PlayerHUD->NoteUI->IsVisible())
	{
		PlayerHUD->NoteUI->SetVisibility(ESlateVisibility::Hidden);
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
	}
	else
	{
		UWidgetBlueprintLibrary::SetFocusToGameViewport();
		PlayerHUD->NoteUI->SetVisibility(ESlateVisibility::Visible);
		PC->SetInputMode(FInputModeGameAndUI());
		PC->SetShowMouseCursor(true);
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
	ECollisionChannel TraceChannel = ECC_Visibility;
	// 트레이스 파라미터 설정
	FHitResult HitResult;
	
	FCollisionQueryParams TraceParams;

	//bool SweepSingleByChannel(struct FHitResult& OutHit, const FVector& Start, const FVector& End, const FQuat& Rot, ECollisionChannel TraceChannel, const FCollisionShape& CollisionShape, const FCollisionQueryParams& Params = FCollisionQueryParams::DefaultQueryParam, const FCollisionResponseParams& ResponseParam = FCollisionResponseParams::DefaultResponseParam) const;

	bool bHit = GetWorld()->SweepSingleByChannel(
		 HitResult,
		 TraceStart,
		 TraceEnd,
		 FQuat::Identity, // 회전 없음
		 TraceChannel,
		 FCollisionShape::MakeSphere(SphereRadius),
		 TraceParams // 탐색 방법에 대한 설정 값을 모아둔 구조체
	 );

	// 디버그용으로 트레이스 구체를 그립니다
	FColor TraceColor = bHit ? FColor::Red : FColor::Green;
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, TraceColor, false, 2.0f, 0, 2.0f);
	DrawDebugSphere(GetWorld(), TraceStart, SphereRadius, 12, TraceColor, false, 2.0f);
	DrawDebugSphere(GetWorld(), TraceEnd, SphereRadius, 12, TraceColor, false, 2.0f);
}
