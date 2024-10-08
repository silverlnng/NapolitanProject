// Fill out your copyright notice in the Description page of Project Settings.


#include "TestCharacter_YJ.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ATestCharacter_YJ::ATestCharacter_YJ()
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
	CrouchingCapsuleHalfHeight = StandingCapsuleHalfHeight / 2.0f;  // Example value, adjust as needed
	StandingWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	CrouchingWalkSpeed = StandingWalkSpeed / 2.0f;  // Example value, adjust as needed

	// Ensure crouch isn't blocking movement
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
}

void ATestCharacter_YJ::BeginPlay()
{
	Super::BeginPlay();
}

void ATestCharacter_YJ::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}


void ATestCharacter_YJ::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATestCharacter_YJ::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATestCharacter_YJ::Look);

		EnhancedInputComponent->BindAction(IA_Crouched, ETriggerEvent::Started, this, &ATestCharacter_YJ::CrouchToggle);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATestCharacter_YJ::Move(const FInputActionValue& Value)
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

void ATestCharacter_YJ::Look(const FInputActionValue& Value)
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

void ATestCharacter_YJ::StartCrouch()
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(CrouchingCapsuleHalfHeight);
	GetCharacterMovement()->MaxWalkSpeed = CrouchingWalkSpeed;

	// Crouch down using Character's built-in function
	Crouch();
}

void ATestCharacter_YJ::StopCrouch()
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(StandingCapsuleHalfHeight);
	GetCharacterMovement()->MaxWalkSpeed = StandingWalkSpeed;

	// Stand up using Character's built-in function
	UnCrouch();
}

void ATestCharacter_YJ::CrouchToggle(const FInputActionValue& Value)
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