// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TestGameInstance.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	bool IsHad;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	int32 ItemInt;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	UTexture2D* thumnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	UStaticMesh* ItemMesh;
};

USTRUCT(BlueprintType)
struct FSouvenirData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	bool IsHad;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	int32 SouvenirInt;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	UTexture2D* thumnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	FString SouvenirInfo;
};

UCLASS()
class NAPOLITANPROJECT_API UTestGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;

//////////// 소모성 아이템 데이터 ///////////////////////	
	UPROPERTY(VisibleAnywhere)
	UDataTable* DT_itemData;
	UPROPERTY(VisibleAnywhere)
	TArray<FName> itemDataRowNames;
	
	//////////// 유물 데이터 ///////////////////////	
	UPROPERTY(VisibleAnywhere)
	UDataTable* DT_SouvenirData;
	UPROPERTY(VisibleAnywhere)
	TArray<FName> SouvenirDataRowNames;

	//////////// 언어 선택 ///////////////////////
	UPROPERTY(VisibleAnywhere)
	int32 lang;

	UFUNCTION()
	void SetGameInstanceLang(int32 value);

	UFUNCTION()
	int32 GetGameInstanceLang();
	
};
