// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyB_FSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyBState : uint8
{
	Idle,
	Move,
	Attack,
	Damage,
	Die,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUBWAY_API UEnemyB_FSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyB_FSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	//me & target & CharacterMovement
	UPROPERTY()
	class AEnemyB* me;

	UPROPERTY(EditAnywhere, Category = Target)
	class AVR_Player* target;
	//class AFPSPlayer* target;

	// Enemy 가 사용하고 있는 AIController 기억
	UPROPERTY()
	class AAIController* ai;

	UPROPERTY(EditAnywhere, Category = FSM, BlueprintReadWrite)
	EEnemyBState m_state_B;
	UPROPERTY()
	class UEnemyBAnimInstance* anim;
public:
	//boolean
	bool bCanDie;
	bool bCanHit;

	//시간
	UPROPERTY(EditAnywhere, Category = FSM)
	float idleDelayTime = 5;
	UPROPERTY(EditAnywhere, Category = FSM)
	float TransADelayTime = 4;
	UPROPERTY()
	float currentTime = 0;
	UPROPERTY(EditAnywhere, Category = FSM)
	float attackDelayTime = 1;

	//범위
	UPROPERTY(EditAnywhere, Category = FSM)
	float attackRange = 150;

	//Boss Movement
	UPROPERTY(EditAnywhere, Category = EnemyBStats)
	float walkSpeed = 80;
	UPROPERTY(EditAnywhere, Category = EnemyBStats)
	float RunSpeed = 180;

	// health System
	UPROPERTY(EditAnywhere, Category = FSM)
	int Health = 10;

	// 피격 받을 때 넉백 힘
	UPROPERTY(EditAnywhere, Category = FSM)
	float knockback = 50;

	// 피격 받을 때 넉백 종료 지점
	UPROPERTY()
	FVector knockbackPos;

private:
	void IdleState();
	void MoveState();
	void AttackState();
	void DamageState();
	void DieState();

public:
	// 피격 함수 (데미지, 넉백 방향)
	void OnDamageProcess(float damage, FVector KBDirection, bool isHead);

	void Die();
	FTimerHandle DieTimerHandle;
		
};
