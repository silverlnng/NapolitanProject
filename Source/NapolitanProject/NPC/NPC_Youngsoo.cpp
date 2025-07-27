// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Youngsoo.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"
#include "NYS_Choice.h"
#include "../GameFrameWork/TestPlayerController.h"
#include "YoungsooAnim.h"
#include "Kismet/GameplayStatics.h"
#include "../GameFrameWork/TestCharacter.h"
#include "../GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/Interact/Souvenir/SouvenirActor.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include <Runtime/LevelSequence/Public/LevelSequencePlayer.h>

#include "NapolitanProject/GameFrameWork/SaveGISubsystem.h"

// Sets default values
ANPC_Youngsoo::ANPC_Youngsoo()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	anim = Cast<UYoungsooAnim>(GetMesh()->GetAnimInstance());

	Scarf = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("scarf"));
	//ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Game/Bada/Asset/Youngsoo/Scarf.Scarf'"));
	
	Scarf->SetupAttachment(GetMesh(), FName(TEXT("neck_01Socket")));
	//Scarf->SetRelativeLocationAndRotation(FVector(4.f, 1.f,110.f),FRotator(90, 0, 110));
	//(X=2.000000,Y=1.000000,Z=1.500000)
}

// Called when the game starts or when spawned
void ANPC_Youngsoo::BeginPlay()
{
	Super::BeginPlay();

	//시작 시 메쉬 숨김
	GetMesh()->SetHiddenInGame(false);
	//GetComponentByClass<UCapsuleComponent>()->SetCollisionProfileName(FName("ClearedNPC"));
	GetComponentByClass<UCapsuleComponent>()->SetCollisionProfileName(FName("NPC"));
	Scarf->SetHiddenInGame(false);

	// playerCharacter 초기화
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		playerCharacter = Cast<ATestCharacter>(PC->GetPawn());
		if (!playerCharacter)
		{
			UE_LOG(LogTemp, Error, TEXT("playerCharacter is nullptr. Ensure your pawn is of type ATestCharacter."));
		}
	}
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

void ANPC_Youngsoo::Interact()
{
	Super::Interact();
}

int32 ANPC_Youngsoo::GetNPCID()
{
	return NPC_ID;
}

int32 ANPC_Youngsoo::GetState()
{
	return State;
}

void ANPC_Youngsoo::ResultEvent(int32 result)
{
	// State 와 선택지의  result 에 따라 이벤트 정의하기
	// 걸어가면서 사라짐 : 0 , 유품 획득 : 1, 에반스매듭 사망엔딩 : 2 

	if (1==State)
	{
		if (0 == result)
		{
			//남자를 위로해주자. “괜찮으십니까”의 경우 => 남자는 소리를 지르다가 사라지고, 물 나이아가라 발생
			
			//애니메이션 블루프린트 실행
			anim = Cast<UYoungsooAnim>(GetMesh()->GetAnimInstance());
			anim->playYSSadMontage(); //슬퍼하는 애니메이션 작동
			UGameplayStatics::PlaySound2D(GetWorld(), YSScreamSound);
			
			// 5초 후에 캐릭터를 숨기기 위한 타이머 설정
			GetWorldTimerManager().SetTimer(TimerHandle, [this]()
			{
				FString dissolve = "Black";
				DissolveEvent(dissolve);
				ChangeCleared();
			}, 4.0f, false);

			//물 시퀀스 재생
			FTimerHandle SouvenirTimer;
			GetWorldTimerManager().SetTimer(SouvenirTimer, [this]()
			{
				FTimerHandle SeTimer;
				ALevelSequenceActor* outActor;
				ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), YoungSooSequence, FMovieSceneSequencePlaybackSettings(), outActor);
				
				if(YoungSooSequence)
				{
					//안보이게 한 후 시퀀스 재생
					playerCharacter->GetMesh()->SetHiddenInGame(true);
					SequencePlayer->Play();
					FTimerHandle finishTimer;
					//시퀀스 재생 종료시 캐릭터 다시 보임
					GetWorldTimerManager().SetTimer(finishTimer, [this]()
					{
						playerCharacter->GetMesh()->SetHiddenInGame(false);
					}, 17.f, false);
				}
			}, 8.0f, false);
			
			
		}
		else if (1 == result)
		{
			// “단 한명만이 닻을 풀어 익사했다." => 내 가족들은... 이라는 대사
			// DynamicMaterial 생성 및 적용
			/*int32 key=(NPC_ID*100)+(State*10)+result;
			PlayerHUD->NPCDialogueUI->SetVisibility(ESlateVisibility::Visible);
			TestPC->SetCurNPCResultUI(key);*/
			
			//유품 스폰

			//캐릭터 사라짐
			GetWorldTimerManager().SetTimer(TimerHandle, [this]()
			{
				FString dissolve = "Yellow";
				DissolveEvent(dissolve); //디졸브 이벤트 발생
				TestPC->StartEndNPCDialougue(false); //결과 대화창 출력
				ChangeCleared();
				Scarf->SetVisibility(false);
			}, 4.0f, false);

		
			//아이템 획득
			FTimerHandle SouvenirTimer;
			GetWorldTimerManager().SetTimer(SouvenirTimer, [this]()
			{
				FTimerHandle SeTimer;
				ALevelSequenceActor* outActor;
				ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), YoungSooSequence, FMovieSceneSequencePlaybackSettings(), outActor);
				
				if(YoungSooSequence)
				{
					//안보이게 한 후 시퀀스 재생
					playerCharacter->GetMesh()->SetHiddenInGame(true);
					SequencePlayer->Play();
					SpawnItems();
					//SequencePlayer->OnFinished.AddDynamic(this,)
					FTimerHandle finishTimer;
					//시퀀스 재생 종료시 캐릭터 다시 보임
					GetWorldTimerManager().SetTimer(finishTimer, [this]()
					{
						playerCharacter->GetMesh()->SetHiddenInGame(false);
					}, 17.f, false);
				}
			}, 8.0f, false);
			
			SaveGI->AcquireSouvenir.Add(NPC_ID);
			
			TestPC->SetSouvenirUICurNumber(NPC_ID);
		}
		else if (2 == result)
		{
			//남자에게 말을 걸어선 안된다. 어서 도망치자.의 경우 => 도망칠 경우 디졸브 블랙 후 NPC 사라짐

			//FString dissolve = "Black";
			//DissolveEvent(dissolve); //디졸브 이벤트 발생
			TestPC->StartEndNPCDialougue(false); //결과 대화창 출력
			ChangeCleared();
			Scarf->SetHiddenInGame(true);
			LiquidMeshChange();
			
		}
		else
		{
			//"위대한 빨간 등대를 부수자" 선택 후
			MainCharacter->GetMesh()->SetHiddenInGame(false);
			TestPC->CameraViewChangePlayer();
			//위대한 빨간 등대를 부수자 -> 카메라 시점 변경 및 플레이어 얼굴을 클로즈 업
			MainCharacter->AdjustCameraPosition(); //시점 변경
			// 글자로 된 에반스 매듭이 나와서 플레이어 목 감싸고 피 터지는 연출과 함께 사망
			GetWorldTimerManager().SetTimer(TimerHandle, [this]()
			{
				PlayerHUD->CreateYSEvance(); //=교수형 UI 출력
				//UI 애니메이션 재생 후 카메라는 고정, 목을 매달는 애니메이션 재생
				FTimerHandle DeadTimer;
				GetWorldTimerManager().SetTimer(DeadTimer, [this]()
				{
					MainCharacter->HangNeckUp(); //메쉬가 위로 올라감
				}, 13.f, false);
				
			}, 7.0f, false);
		}
	}
	else if (2 == State)
	{
		
	}
	else if (3 == State)
	{
		
	}
	
}

void ANPC_Youngsoo::ChangeCleared()
{
	Super::ChangeCleared();
}

void ANPC_Youngsoo::DissolveEvent(FString& str)
{
	Super::DissolveEvent(str);
}

void ANPC_Youngsoo::SpawnItems()
{
	// 발끝 위치를 기준으로 스폰 위치 설정
	FVector FootLocation = GetMesh()->GetSocketLocation(FName("ItemSpawn"));
	FTransform SpawnTransform(FootLocation);

	// 블루프린트에서 설정된 ItemClass와 SouvenirClass로 스폰
	AActor* SouvenirActor = GetWorld()->SpawnActor<ASouvenirActor>(SouvenirClass, SpawnTransform );
	//AActor* ItemActor = GetWorld()->SpawnActor<AItemActor>(ItemClass, SpawnTransform);

	if (SouvenirActor)
	{
		//ItemActor->Tags.Add(FName("Item"));
		SouvenirActor->Tags.Add(FName("Souvenir"));
		Destroy();
	}		
}

void ANPC_Youngsoo::LiquidMeshChange()
{
	Scarf->SetVisibility(true);
	GetMesh()->SetHiddenInGame(true);
	
	UE_LOG(LogTemp, Warning, TEXT("=== Moving LiquidMesh actor to my location ==="));

	// 현재 액터(me)의 위치와 회전 가져오기
	FVector MyLocation = GetMesh()->GetComponentLocation();
	FRotator MyRotation = GetMesh()->GetComponentRotation();
	
	UE_LOG(LogTemp, Warning, TEXT("My Location: %s"), *MyLocation.ToString());

	// "LiquidMesh" 태그를 가진 액터 찾기
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("LiquidMesh"), FoundActors);
	
	if (FoundActors.Num() > 0)
	{
		AActor* LiquidActor = FoundActors[0]; // 첫 번째 찾은 액터 사용
		
		UE_LOG(LogTemp, Warning, TEXT("Found LiquidMesh actor: %s"), *LiquidActor->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Moving from: %s"), *LiquidActor->GetActorLocation().ToString());
		UE_LOG(LogTemp, Warning, TEXT("Moving to: %s"), *MyLocation.ToString());
		
		// 액터를 내 위치로 이동
		LiquidActor->SetActorLocationAndRotation(MyLocation, MyRotation);
		
		// 액터가 숨겨져 있다면 보이게 설정
		if (LiquidActor->IsHidden())
		{
			LiquidActor->SetActorHiddenInGame(false);
			UE_LOG(LogTemp, Warning, TEXT("Made hidden LiquidMesh actor visible"));
		}
		
		// 별도로 BP_Liquid 액터 찾아서 LiquidEffect 함수 호출
		TArray<AActor*> BP_LiquidActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("BP_Liquid"), BP_LiquidActors);
		
		if (BP_LiquidActors.Num() > 0)
		{
			AActor* BP_LiquidActor = BP_LiquidActors[0];
			UFunction* LiquidEffectFunction = BP_LiquidActor->FindFunction(FName("LiquidEffect"));
			if (LiquidEffectFunction)
			{
				BP_LiquidActor->ProcessEvent(LiquidEffectFunction, nullptr);
				UE_LOG(LogTemp, Warning, TEXT("LiquidEffect function called on BP_Liquid actor successfully!"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("LiquidEffect function not found in BP_Liquid actor: %s"), *BP_LiquidActor->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("BP_Liquid actor not found! Please add 'BP_Liquid' tag to the BP_Liquid actor."));
		}
		
		UE_LOG(LogTemp, Warning, TEXT("LiquidMesh actor moved and activated successfully!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No actor found with tag 'LiquidMesh'! Please add the tag to your BP_Liquid actor."));
	}
}


