// Fill out your copyright notice in the Description page of Project Settings.


#include "Picture_Zombie.h"

#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APicture_Zombie::APicture_Zombie()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);

	BoxComp=CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComponent"));
	BoxComp->SetupAttachment(RootComponent);

	
	StaticMeshComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComp->SetupAttachment(BoxComp);
	
	CapsuleComp=CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CapsuleComp->SetupAttachment(RootComponent);

	SkeletalMeshComp1=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp1"));
	SkeletalMeshComp1->SetupAttachment(CapsuleComp);
}

// Called when the game starts or when spawned
void APicture_Zombie::BeginPlay()
{
	Super::BeginPlay();

	// 스켈레탈 메쉬는 아예 안보이도록

	SkeletalMeshComp1->SetHiddenInGame(true);
}

// Called every frame
void APicture_Zombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

 void APicture_Zombie::DropEffect()
{
	StaticMeshComp->SetSimulatePhysics(true);
	
	// 몇초 후 좀비의 상반신 만 보이도록 수정
	
	FTimerHandle nextTimer;

	GetWorld()->GetTimerManager().SetTimer(nextTimer,[this]()
	{
		
	},1.0f,false);
}





