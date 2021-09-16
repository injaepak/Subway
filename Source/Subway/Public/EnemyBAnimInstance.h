// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyBAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SUBWAY_API UEnemyBAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = FSM, BlueprintReadOnly)
	bool isMoving = false;
	UPROPERTY(VisibleAnywhere, Category = FSM, BlueprintReadOnly)
	bool isAttacking = false;
	UPROPERTY(VisibleAnywhere, Category = FSM, BlueprintReadOnly)
	bool isDie = false;
};