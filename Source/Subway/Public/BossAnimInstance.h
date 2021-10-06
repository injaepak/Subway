// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SUBWAY_API UBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, Category = FSM, BlueprintReadOnly)
	bool isTrans = false;
	UPROPERTY(VisibleAnywhere, Category = FSM, BlueprintReadOnly)
	bool isTaunt = false;
	UPROPERTY(VisibleAnywhere, Category = FSM, BlueprintReadOnly)
	bool isMoving = false;
	UPROPERTY(VisibleAnywhere, Category = FSM, BlueprintReadOnly)
	bool isDamage = false;
	UPROPERTY(VisibleAnywhere, Category = FSM, BlueprintReadOnly)
	bool isAttacking = false;
	UPROPERTY(VisibleAnywhere, Category = FSM, BlueprintReadOnly)
	bool isDie = false;
};
