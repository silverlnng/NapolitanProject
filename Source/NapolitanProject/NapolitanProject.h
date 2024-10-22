// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define PRINTLOG(fmt, ...) UE_LOG(NetLog, Warning, TEXT("[%s]%s : %s"), NETMODE, *CALLINFO, *FString::Printf(fmt, ##__VA_ARGS__));