// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetForItem.h"
#include "TargetForItem_LobbyDoor.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API ATargetForItem_LobbyDoor : public ATargetForItem
{
	GENERATED_BODY()

	virtual void CheckItem(class AItemActor* curItem)override;
	
	virtual void CheckItemSuccess() override;
	
	// 상호작용하는 아이템에대해 미리 설정
	//UPROPERTY(EditAnywhere)
	//int32 TargetItemID=0;

	virtual void CheckItemFail()override;
	
	virtual void NoItem()override;
};
