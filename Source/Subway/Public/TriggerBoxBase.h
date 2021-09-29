// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "TriggerBoxBase.generated.h"

/**
 * 
 */
UCLASS()
class SUBWAY_API ATriggerBoxBase : public ATriggerBox
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Test")
		bool openTheDoor = false;
	
};
