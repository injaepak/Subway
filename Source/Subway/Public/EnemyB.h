// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyB_FSM.h"
#include "GameFramework/Character.h"
#include "EnemyB.generated.h"

UCLASS()
class SUBWAY_API AEnemyB : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyB();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, Category = EnemyAFSM, BlueprintReadWrite)
	class UEnemyB_FSM* enemyBFSM;

public:
	// Health System
	bool bCanBeDamaged;
};
