// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Youngsoo.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"
#include "NYS_Choice.h"
#include "Kismet/GameplayStatics.h"
#include "../YJ/TestCharacter.h"
#include "../YJ/PlayerHUD.h"

// Sets default values
ANPC_Youngsoo::ANPC_Youngsoo()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	YSCol = CreateDefaultSubobject<UCapsuleComponent>(TEXT("YSCol"));
	//YSCol->SetCollisionProfileName(FName("YSCol"));
	GetCapsuleComponent()->SetCapsuleHalfHeight(110.f);
	GetCapsuleComponent()->SetCapsuleRadius(60.f);
}

// Called when the game starts or when spawned
void ANPC_Youngsoo::BeginPlay()
{
	Super::BeginPlay();
	
	YSCol->OnComponentBeginOverlap.AddDynamic(this, &ANPC_Youngsoo::ChoiceUIOverlap);

	//플레이어 캐스팅
	player = CastChecked<ATestCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	auto* pc = player->GetController<APlayerController>();
	huds = pc->GetHUD<APlayerHUD>();

}

// Called every frame
void ANPC_Youngsoo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPC_Youngsoo::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//미술 작품을 보면서 울고 있는 경우
void ANPC_Youngsoo::YSFirstState()
{
	//대기 상태로 우는 애니메이션 재생
}

//스카프를 불태워야하는 경우
void ANPC_Youngsoo::YSSecondState()
{
	//우선은 대기 상태로 있다가 플레이어가 다가오면 다음 함수를 오버랩으로 부른다. 
}

//남자가 작품을 보며 웃을 때 -> 그 영역 안에 들어왓을 때 추적 후 무조건 사망
void ANPC_Youngsoo::YSThirdState()
{
	//우선은 대기 상태로 웃고있는 애니메이션을 작동
}

//랜덤으로 상태지정
NYSState ANPC_Youngsoo::YSRandomState()
{
	//랜덤으로 상태 지정
	TArray<NYSState> RanStates = {
		NYSState::YSFirst,
		NYSState::YSSecond,
		NYSState::YSThird,
	};

	//랜덤으로 선택
	int32 RandomIndex = FMath::RandRange(0, RanStates.Num() - 1);
	NYSState NewState = RanStates[RandomIndex];

	return NewState;
}

//조건 해금 후 움직인 다음 플레이어의 시야에서 사라짐
void ANPC_Youngsoo::YSMoveState()
{
}

//플레이어 추격 후 플레이어는 반드시 사망
void ANPC_Youngsoo::YSChaseState()
{
}

void ANPC_Youngsoo::SetState(NYSState newState)
{
	mState = newState; //상태 지정
	// 나중에 애니메이션 상태도 변경하기
}

void ANPC_Youngsoo::ChoiceUIOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		//만약 영수와 부딪힌 것이 플레이어일 경우
		auto* AttackTarget = Cast<ATestCharacter>(OtherActor);
		if (AttackTarget) {
			bIsChat = true;
			//bIsChat이 true일 경우에만
			if (bIsChat && mState == NYSState::YSFirst) { 
				initNYSUI(); //딱 한번만 UI가 뜨도록 설정
			}
			else if (bIsChat && mState == NYSState::YSSecond) {
				//스카프를 불태우는 함수 호출
			}
		}
	}
}

void ANPC_Youngsoo::initNYSUI()
{
	if (huds) {
		huds->NYSUi->SetVisibility(ESlateVisibility::Visible);
	}
}

