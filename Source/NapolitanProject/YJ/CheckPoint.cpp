// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPoint.h"

#include "Components/SphereComponent.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
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
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComp->SetCollisionResponseToAllChannels(ECR_Block);

	//CheckpointMesh->SetCustomDepthStencilValue(CustomDepthStencilOverride);
	//CheckpointMesh->MarkRenderStateDirty();

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
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::OnSphereOverlap);
}

// Called every frame
void ACheckPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		MainCharacter->SaveLocation=SaveLocation;
		// ui 보는 모드로 만들기
		TestPC->SetInputMode(FInputModeGameAndUI());
		TestPC->SetShowMouseCursor(true);
		MainCharacter->SetPlayerState(EPlayerState::UI);
	}
}