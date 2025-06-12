// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPoint.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/SaveGISubsystem.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "SaveUI/LoadConfirmWidget.h"
#include "SaveUI/LoadScreenWidget.h"
#include "SaveUI/SaveConfirmWidget.h"

// Sets default values
ACheckPoint::ACheckPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>("Scene");
	SetRootComponent(SceneComp);
	
	SphereComp = CreateDefaultSubobject<USphereComponent>("Sphere");
	SphereComp->SetupAttachment(SceneComp);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("CheckpointMesh");
	StaticMeshComp->SetupAttachment(SphereComp);
	//StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//StaticMeshComp->SetCollisionResponseToAllChannels(ECR_Block);

	SaveLocComp = CreateDefaultSubobject<USceneComponent>("SaveLoc");
	SaveLocComp->SetupAttachment(RootComponent);

	
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CapsuleComp->SetupAttachment(SceneComp);
	CapsuleComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,ECR_Block);
	
}

// Called when the game starts or when spawned
void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();
	TestPC = GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	if (TestPC)
	{
		MainCharacter =TestPC->GetPawn<ATestCharacter>();
		PlayerHUD=TestPC->GetHUD<APlayerHUD>();
	}
	//SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::OnSphereOverlap);

	GI=GetGameInstance<UMyTestGameInstance>();
	SaveGI = GI->GetSubsystem<USaveGISubsystem>();
}

// Called every frame
void ACheckPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACheckPoint::IInteract()
{
	//IInteractInterface::IInteract();
	VisibleSaveWidget();
}

void ACheckPoint::VisibleSaveWidget()
{
	PlayerHUD->LoadScreenUI->SetVisibility(ESlateVisibility::Visible);

	PlayerHUD->LoadScreenUI->SaveLocation=SaveLocation;

	PlayerHUD->LoadScreenUI->SaveConfirmWidget->SaveLocation=SaveLocation;
	PlayerHUD->LoadScreenUI->WBP_LoadConfirm->SaveLocation=SaveLocation;
	MainCharacter->SaveLocationStr=SaveLocation;
	MainCharacter->SaveTransform=SaveLocComp->GetComponentTransform();

	// 로드해야하는 주위 서브레벨들
	SaveGI->SubLevelArray=SubLevelArray;
	
	// ui 보는 모드로 만들기
	TestPC->SetInputMode(FInputModeGameAndUI());
	TestPC->SetShowMouseCursor(true);
	MainCharacter->SetPlayerState(EPlayerState::UI);
}

void ACheckPoint::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// OtherActor 플레이어면 저장 되도록 만들기
	if (OtherActor->IsA(ATestCharacter::StaticClass()))
	{
		PlayerHUD->LoadScreenUI->SetVisibility(ESlateVisibility::Visible);

		PlayerHUD->LoadScreenUI->SaveLocation=SaveLocation;

		PlayerHUD->LoadScreenUI->SaveConfirmWidget->SaveLocation=SaveLocation;
		PlayerHUD->LoadScreenUI->WBP_LoadConfirm->SaveLocation=SaveLocation;
		MainCharacter->SaveLocationStr=SaveLocation;
		MainCharacter->SaveTransform=SaveLocComp->GetComponentTransform();
		
		// 그냥 여기서 자신의 서브레벨 로드하는게 편함. =>느림 
		//GetWorldTimerManager().SetTimer(loadSubLevelTimerHandle, this, &ACheckPoint::ProcessNextSubLevel, 1.0f, false);
		
		// ui 보는 모드로 만들기
		TestPC->SetInputMode(FInputModeGameAndUI());
		TestPC->SetShowMouseCursor(true);
		MainCharacter->SetPlayerState(EPlayerState::UI);
	}
}

void ACheckPoint::ProcessNextSubLevel()
{
	if (CurrentIndex < SubLevelArray.Num())
	{
		FLatentActionInfo LatentAction;
		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(),SubLevelArray[CurrentIndex],true,true,LatentAction);
		
		CurrentIndex++;

		// 다음 아이템을 1초 후 처리
		GetWorldTimerManager().SetTimer(loadSubLevelTimerHandle, this, &ACheckPoint::ProcessNextSubLevel, 1.0f, false);
	}
	else
	{
		// 종료 처리
		UE_LOG(LogTemp, Log, TEXT("모든 레벨 로드 완료"));
	}
}
