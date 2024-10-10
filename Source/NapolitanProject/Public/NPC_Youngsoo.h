// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPC_Youngsoo.generated.h"

//��ü ����
UENUM(BlueprintType)
enum class NYSState : uint8 {
	YSFirst,
	YSSecond,
	YSThird,
	YSRandom,
	YSMove,
	YSChase,
};

UCLASS()
class NAPOLITANPROJECT_API ANPC_Youngsoo : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC_Youngsoo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	//���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	NYSState mState = NYSState::YSRandom; //�ϴ� ó���� ���¸� ���� ����

	//���� �Լ�
	UFUNCTION(BlueprintCallable, Category = State)
	void YSFirstState(); //�̼� ��ǰ�� ���鼭 ��� �ִ� ���

	UFUNCTION(BlueprintCallable, Category = State)
	void YSSecondState(); //��ī���� ���¿����ϴ� ���

	UFUNCTION(BlueprintCallable, Category = State)
	void YSThirdState(); //���ڰ� ��ǰ�� ���� ���� �� -> �� ���� �ȿ� ������ �� ���� �� ������ ���

	UFUNCTION(BlueprintCallable, Category = State)
	NYSState YSRandomState(); //�������� ��������

	UFUNCTION(BlueprintCallable, Category = State)
	void YSMoveState(); //���� �ر� �� ������ ���� �÷��̾��� �þ߿��� �����

	UFUNCTION(BlueprintCallable, Category = State)
	void YSChaseState(); //�̼� ��ǰ�� ���鼭 ��� �ִ� ���

	UFUNCTION()
	void SetState(NYSState newState);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCapsuleComponent* YSCol; // ���� ���� �ÿ� ������ UI ����

	bool bIsChat = false;

	UFUNCTION()
	void ChoiceUIOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);


	UFUNCTION()
	void initNYSUI();

	UPROPERTY()
	class ATestCharacter* player; //�÷��̾�

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class APlayerHUD* hud; //HUD

};
