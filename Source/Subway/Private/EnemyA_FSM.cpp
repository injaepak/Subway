// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyA_FSM.h"
#include "EnemyA.h"
#include "FPSPlayer.h"
#include "EnemyAAnimInstance.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UEnemyA_FSM::UEnemyA_FSM()
{
	PrimaryComponentTick.bCanEverTick = true;
	bCanDie = true;
}

// Called when the game starts
void UEnemyA_FSM::BeginPlay()
{
	Super::BeginPlay();

	// EnemyA = me
	me = Cast<AEnemyA>(GetOwner());
	anim = Cast<UEnemyAAnimInstance>(me->GetMesh()->GetAnimInstance());
	//target 찾기
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFPSPlayer::StaticClass(), actors);
	for (auto tgt : actors)
	{
		target = Cast<AFPSPlayer>(tgt);
		break;
	}
}

// Called every frame
void UEnemyA_FSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (m_state_A)
	{
	case EEnemyAState::Idle:
		IdleState();
		break;
	case EEnemyAState::Move:
		MoveState();
		break;
	case EEnemyAState::Attack:
		AttackState();
		break;
	//case EEnemyAState::Damage:
	//	DamageState();
	//	break;
	case EEnemyAState::Die:
		DieState();
		break;
	}
}

void UEnemyA_FSM::IdleState()
{
	// 상태를 Move로 바꾼다.
	currentTime += GetWorld()->DeltaTimeSeconds;

	// 시간이 지났으니 상태 변경
	if (currentTime > idleDelayTime)
	{
		m_state_A = EEnemyAState::Move;
		// CurrentTime 리셋
		currentTime = 0;
	}
	// HP = 0, isDie가 False라면
	else if (Health == 0 && anim->isDie == false)
	{
		m_state_A = EEnemyAState::Die;
		currentTime = 0;
	}
}

void UEnemyA_FSM::MoveState()
{
	me->GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
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
		m_state_A = EEnemyAState::Attack;
		anim->isMoving = false;
		anim->isAttacking = true;
		currentTime = attackDelayTime;
	}

	// HP = 0, isDie가 False라면
	if (Health == 0 && anim->isDie == false)
	{
		m_state_A = EEnemyAState::Die;
		currentTime = 0;
	}
}

void UEnemyA_FSM::AttackState()
{
	// 시간이 흐른다.
	currentTime += GetWorld()->DeltaTimeSeconds;

	// 일정 시간이 지났으니 공격으로 변경
	if (currentTime > attackDelayTime)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("ATTACK!!")));
		// 시간 초기화
		currentTime = 0;
	}
	// HP = 0, isDie가 False라면
	else if (Health == 0 && anim->isDie == false)
	{
		m_state_A = EEnemyAState::Die;
		currentTime = 0;
	}
}

//void UEnemyA_FSM::DamageState()
//{
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Damaged!!")));
//
//}

void UEnemyA_FSM::DieState()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Dead!!")));
	anim->isMoving = false;
	anim->isAttacking = false;
	anim->isDie = true;
	
	if (bCanDie == true)
	{
		GetWorld()->GetTimerManager().SetTimer(DieTimerHandle, this, &UEnemyA_FSM::Die, 2.5f, false);
		bCanDie = false;
	}
}

void UEnemyA_FSM::OnDamageProcess()
{
	Health--;
	if (Health <= 0)
	{
		m_state_A = EEnemyAState::Die;
		//me->Destroy();
		return;
	}

	//상태를 DamageState로 이동
	//m_state_A = EEnemyAState::Damage;
	//currentTime = 0;
}

void UEnemyA_FSM::Die()
{
	me->Destroy();
}
