// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpScare_Canvas.h"

#include "JumpScare_Picture.h"
#include "Components/BoxComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"

// Sets default values
AJumpScare_Canvas::AJumpScare_Canvas()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);
	
	SceneCaptureComponent2D=CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComp"));
	SceneCaptureComponent2D->SetupAttachment(RootComponent);

	BoxComp = CreateDefaultSubobject<UBoxComponent>("BoxComp");
	BoxComp->SetupAttachment(RootComponent);

	BoxComp2 = CreateDefaultSubobject<UBoxComponent>("BoxComp2");
	BoxComp2->SetupAttachment(RootComponent);

	CanvasStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("ExitDoor");
	CanvasStaticMeshComp->SetupAttachment(RootComponent);

	CanvasBackBoxComp = CreateDefaultSubobject<UBoxComponent>("CanvasBoxComp");
	CanvasBackBoxComp->SetupAttachment(CanvasStaticMeshComp);
}

// Called when the game starts or when spawned
void AJumpScare_Canvas::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this,&AJumpScare_Canvas::OnBeginOverlap);
	BoxComp2->OnComponentBeginOverlap.AddDynamic(this,&AJumpScare_Canvas::OnBeginOverlap3);
	CanvasBackBoxComp->OnComponentBeginOverlap.AddDynamic(this,&AJumpScare_Canvas::OnBeginOverlap2);
	
	// tick을 꺼두기 .
	SceneCaptureComponent2D->bCaptureEveryFrame=false;

	//canvas mesh  의 mat 이 사용하는 캔버스 렌더타겟을 SceneCaptureComp의 렌더타겟으로
	
}

// Called every frame
void AJumpScare_Canvas::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJumpScare_Canvas::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	// 주인공 캐릭터일때 스케어용 캐릭터 작동시키기
	if (OtherActor->IsA(ATestCharacter::StaticClass()))
	{
		if (bHasTriggered)
		{
			return;
		}
		bHasTriggered = true; // 한번만 작동시키도록

		SceneCaptureComponent2D->bCaptureEveryFrame=true;

	
		
		if (JumpScarePicture_Character)
		{
			JumpScarePicture_Character->PlayBasicAnimMontage();
		}
	}	
	
}

void AJumpScare_Canvas::OnBeginOverlap3(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 주인공 캐릭터일때 스케어용 캐릭터 작동시키기
	if (OtherActor->IsA(ATestCharacter::StaticClass()))
	{
		if (bHasTriggered3)
		{
			return;
		}
		bHasTriggered3 = true; // 한번만 작동시키도록

		
		// 여기서 이제 달려오도록 실행 . 
		
		if (JumpScarePicture_Character)
		{
			// 앞으로 이동 
			JumpScarePicture_Character->bIsMovingForward =true;
			// 다른애니메이션( 공격애니메이션을) 실행 ==> 오직한번만실행.
			JumpScarePicture_Character->PlayRunAnimMontage();
			
		}
	}	
}


// 그림뒤에 점프스케어 캐릭터가 다왔을때 
void AJumpScare_Canvas::OnBeginOverlap2(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AJumpScare_Picture::StaticClass())) 
	{
		if (bHasTriggered2)
		{
			return;
		}
		
		bHasTriggered2 = true; // 한번만 작동시키도록

		JumpScarePicture_Character->bIsMovingForward =false;
		
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "JumpScare_Canvas::OnBeginOverlap2");
		
		// JumpScarePicture_Character 를 이제 튀어나오는 애니메이션 작동시키도록
		if (JumpScarePicture_Character)
		{
			JumpScarePicture_Character->PlayJumpAttackAnimMontage();
		}
	}	
}

void AJumpScare_Canvas::StartJumpScare()
{
	//
	SceneCaptureComponent2D->bCaptureEveryFrame=true;
}
