// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyA_FSM.h"
#include "GameFramework/Character.h"
#include "EnemyA.generated.h"

UCLASS()
class SUBWAY_API AEnemyA : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyA();

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
	class UEnemyA_FSM* enemyAFSM;

public:
	// Health System
	bool bCanBeDamaged;

	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)		float EnemyAMaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float EnemyAHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float EnemyAHealthPercentage;
	*/
};
