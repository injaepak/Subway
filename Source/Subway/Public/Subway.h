// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(Subway, Log, All);

#define APPINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define CALLINFO() UE_LOG(Subway, Warning, TEXT("%s"), *APPINFO)
#define PRINTLOG(fmt, ...) UE_LOG(Subway, Warning, TEXT("%s %s"), *APPINFO, *FString::Printf(fmt, ##__VA_ARGS__) )
