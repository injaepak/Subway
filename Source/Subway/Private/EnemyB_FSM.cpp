// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyB_FSM.h"
#include "EnemyB.h"
#include "FPSPlayer.h"
#include "VR_Player.h"
#include "EnemyBAnimInstance.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UEnemyB_FSM::UEnemyB_FSM()
{
	PrimaryComponentTick.bCanEverTick = true;
	bCanDie = true;
	//bCanHit = false;
}


// Called when the game starts
void UEnemyB_FSM::BeginPlay()
{
	Super::BeginPlay();
	// EnemyB = me
	me = Cast<AEnemyB>(GetOwner());
	anim = Cast<UEnemyBAnimInstance>(me->GetMesh()->GetAnimInstance());
	//target 찾기
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVR_Player::StaticClass(), actors);
	for (auto tgt : actors)
	{
		target = Cast<AVR_Player>(tgt);
		break;
	}
}


// Called every frame
void UEnemyB_FSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	switch (m_state_B)
	{
	case EEnemyBState::Idle:
		IdleState();
		break;
	case EEnemyBState::Move:
		MoveState();
		break;
	case EEnemyBState::Attack:
		AttackState();
		break;
	//case EEnemyBState::Damage:
	//	DamageState();
	//	break;
	case EEnemyBState::Die:
		DieState();
		break;
	}
}

void UEnemyB_FSM::IdleState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;

	// 시간이 지났으니 상태 변경
	if (currentTime > idleDelayTime)
	{
		m_state_B = EEnemyBState::Move;
		// CurrentTime 리셋
		currentTime = 0;
	}
	// HP = 0, isDie가 False라면
	else if (Health == 0 && anim->isDie == false)
	{
		m_state_B = EEnemyBState::Die;
		currentTime = 0;
	}
}

void UEnemyB_FSM::MoveState()
{
	me->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	// Target인 Player 방향으로 이동
	// 	방향이 필요
	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	// 둘사이의 거리
	float distance = dir.Size();
	dir.Normalize();

	// character movement로 사용해서 따라가게 만들기
	me->AddMovementInput(dir, true);

	// 타겟 방향으로 회전한다.		
	FRotator targetRot = dir.ToOrientationRotator();
	FRotator myRot = me->GetActorRotation();

	//회전 부드럽게
	myRot = FMath::Lerp(myRot, targetRot, 5 * GetWorld()->DeltaTimeSeconds);
	me->SetActorRotation(myRot);

	//Debug Sphere 시각화
	DrawDebugSphere(GetWorld(), me->GetActorLocation(), attackRange, 8, FColor::Red);

	// 속도가 있을 때, AnimInstance Bool 변경
	if (anim->isMoving == false)
	{
		//속도 구하기
		float velocity = me->GetVelocity().Size();
		if (velocity > 0.1f)
		{
			anim->isMoving = true;
		}
	}

	// state 경과
	// 공격 범위에 가까워지면
	if (distance <= attackRange)
	{
		m_state_B = EEnemyBState::Attack;
		anim->isMoving = false;
		anim->isAttacking = true;
		currentTime = attackDelayTime;
	}

	// HP = 0, isDie가 False라면
	if (Health == 0 && anim->isDie == false)
	{
		m_state_B = EEnemyBState::Die;
		currentTime = 0;
	}
}

void UEnemyB_FSM::AttackState()
{
	me->bCanAttack = true;
	// 시간이 흐른다.
	currentTime += GetWorld()->DeltaTimeSeconds;

	// 일정 시간이 지났으니 공격으로 변경
	if (currentTime > attackDelayTime)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("ATTACK!!")));
		// 시간 초기화
		currentTime = 0;
	}
	// HP = 0, isDie가 False라면
	else if (Health == 0 && anim->isDie == false)
	{
		m_state_B = EEnemyBState::Die;
		currentTime = 0;
	}
}

//void UEnemyB_FSM::DamageState()
//{
//
//}

void UEnemyB_FSM::DieState()
{
	me->bCanAttack = false;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Dead!!")));
	//anim->isTransA = false;
	anim->isMoving = false;
	anim->isAttacking = false;
	anim->isDie = true;

	if (bCanDie == true)
	{
		GetWorld()->GetTimerManager().SetTimer(DieTimerHandle, this, &UEnemyB_FSM::Die, 2.5f, false);
		bCanDie = false;
	}
}

void UEnemyB_FSM::OnDamageProcess(float damage)
{
	if (Health > 0)
	{
		if (bCanHit == false)
		{
			bCanHit = true;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("bCanHit!!")));
			return;
		}

		Health -= damage;
		if (Health < 0)
		{
			m_state_B = EEnemyBState::Die;
			//me->Destroy();
			return;
		}
	}
}

void UEnemyB_FSM::Die()
{
	me->Destroy();
}

