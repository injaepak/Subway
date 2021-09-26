// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boss_FSM.h"
#include "GameFramework/Character.h"
#include "Boss.generated.h"

UCLASS()
class SUBWAY_API ABoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	UPROPERTY(VisibleAnywhere, Category = BossFSM, BlueprintReadWrite)
	class UBoss_FSM* BossFSM;

public:
	// Health System
	bool bCanBeDamaged;
};
