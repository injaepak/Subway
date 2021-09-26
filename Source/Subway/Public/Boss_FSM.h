// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Boss_FSM.generated.h"

UENUM(BlueprintType)
enum class EBossState : uint8
{
	Idle,
	Trans,
	Taunt,
	Move,
	Attack,
	Damage,
	Die,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUBWAY_API UBoss_FSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBoss_FSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	//me & target & CharacterMovement
	UPROPERTY()
	class ABoss* me;

	UPROPERTY(EditAnywhere, Category = Target)
	class AVR_Player* target;
	//class AFPSPlayer* target;

	UPROPERTY(EditAnywhere, Category = FSM, BlueprintReadWrite)
	EBossState m_state_Boss;
	UPROPERTY()
	class UBossAnimInstance* anim;
public:
	//boolean
	bool bCanDie;

	//시간
	UPROPERTY(EditAnywhere, Category = FSM)
	float idleDelayTime = 3;
	UPROPERTY(EditAnywhere, Category = FSM)
	float TransDelayTime = 1.5;
	UPROPERTY(EditAnywhere, Category = FSM)
	float TauntDelayTime = 1;
	UPROPERTY()
	float currentTime = 0;
	UPROPERTY(EditAnywhere, Category = FSM)
	float attackDelayTime = 1;

	//범위
	UPROPERTY(EditAnywhere, Category = FSM)
	float attackRange = 100;

	//Boss Movement
	UPROPERTY(EditAnywhere, Category = EnemyAStats)
	float walkSpeed = 150;
	UPROPERTY(EditAnywhere, Category = EnemyAStats)
	float RunSpeed = 250;

	// health System
	UPROPERTY(EditAnywhere, Category = FSM)
	int Health = 25;
private:
	void IdleState();
	void TransState();
	void TauntState();
	void MoveState();
	void AttackState();
	//void DamageState();
	void DieState();

public:
	// 피격 함수
	void OnDamageProcess();

	void Die();
	FTimerHandle DieTimerHandle;
		
};
