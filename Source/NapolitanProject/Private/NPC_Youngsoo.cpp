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

	//�÷��̾� ĳ����
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

//�̼� ��ǰ�� ���鼭 ��� �ִ� ���
void ANPC_Youngsoo::YSFirstState()
{
	//��� ���·� ��� �ִϸ��̼� ���
}

//��ī���� ���¿����ϴ� ���
void ANPC_Youngsoo::YSSecondState()
{
	//�켱�� ��� ���·� �ִٰ� �÷��̾ �ٰ����� ���� �Լ��� ���������� �θ���. 
}

//���ڰ� ��ǰ�� ���� ���� �� -> �� ���� �ȿ� ������ �� ���� �� ������ ���
void ANPC_Youngsoo::YSThirdState()
{
	//�켱�� ��� ���·� �����ִ� �ִϸ��̼��� �۵�
}

//�������� ��������
NYSState ANPC_Youngsoo::YSRandomState()
{
	//�������� ���� ����
	TArray<NYSState> RanStates = {
		NYSState::YSFirst,
		NYSState::YSSecond,
		NYSState::YSThird,
	};

	//�������� ����
	int32 RandomIndex = FMath::RandRange(0, RanStates.Num() - 1);
	NYSState NewState = RanStates[RandomIndex];

	return NewState;
}

//���� �ر� �� ������ ���� �÷��̾��� �þ߿��� �����
void ANPC_Youngsoo::YSMoveState()
{
}

//�÷��̾� �߰� �� �÷��̾�� �ݵ�� ���
void ANPC_Youngsoo::YSChaseState()
{
}

void ANPC_Youngsoo::SetState(NYSState newState)
{
	mState = newState; //���� ����
	// ���߿� �ִϸ��̼� ���µ� �����ϱ�
}

void ANPC_Youngsoo::ChoiceUIOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		//���� ������ �ε��� ���� �÷��̾��� ���
		auto* AttackTarget = Cast<ATestCharacter>(OtherActor);
		if (AttackTarget) {
			bIsChat = true;
			//bIsChat�� true�� ��쿡��
			if (bIsChat && mState == NYSState::YSFirst) { 
				initNYSUI(); //�� �ѹ��� UI�� �ߵ��� ����
			}
			else if (bIsChat && mState == NYSState::YSSecond) {
				//��ī���� ���¿�� �Լ� ȣ��
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

