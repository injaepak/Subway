// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyA_FSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyAState : uint8
{
	Idle,
	Move,
	Attack,
	Damage,
	Die,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUBWAY_API UEnemyA_FSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyA_FSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	//me & target & CharacterMovement
	UPROPERTY()
	class AEnemyA* me;

	UPROPERTY(EditAnywhere, Category = Target)
	class AVR_Player* target;
	//class AFPSPlayer* target;

	UPROPERTY(EditAnywhere, Category = FSM, BlueprintReadWrite)
	EEnemyAState m_state_A;
	UPROPERTY()
	class UEnemyAAnimInstance* anim;
public:
	//boolean
	bool bCanDie;

	//�ð�
	UPROPERTY(EditAnywhere, Category = FSM)
	float idleDelayTime = 2;
	UPROPERTY()
	float currentTime = 0;
	UPROPERTY(EditAnywhere, Category = FSM)
	float attackDelayTime = 1;

	//����
	UPROPERTY(EditAnywhere, Category = FSM)
	float attackRange = 100;

	//Boss Movement
	UPROPERTY(EditAnywhere, Category = EnemyAStats)
	float walkSpeed = 50;
	UPROPERTY(EditAnywhere, Category = EnemyAStats)
	float RunSpeed = 100;

	// health System
	UPROPERTY(EditAnywhere, Category = FSM)
	int Health = 5;
private:
	void IdleState();
	void MoveState();
	void AttackState();
	//void DamageState();
	void DieState();

public:
	// �ǰ� �Լ�
	void OnDamageProcess();	
	
	void Die();
	FTimerHandle DieTimerHandle;
};
