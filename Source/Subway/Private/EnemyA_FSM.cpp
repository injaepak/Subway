// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyA_FSM.h"
#include "EnemyA.h"
#include "FPSPlayer.h"
#include "VR_Player.h"
#include "EnemyAAnimInstance.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "NavigationInvokerComponent.h"

// Sets default values for this component's properties
UEnemyA_FSM::UEnemyA_FSM()
{
	PrimaryComponentTick.bCanEverTick = true;
	bCanDie = true;
	bCanHit = false;

}

// Called when the game starts
void UEnemyA_FSM::BeginPlay()
{
	Super::BeginPlay();

	// EnemyA = me
	me = Cast<AEnemyA>(GetOwner());
	anim = Cast<UEnemyAAnimInstance>(me->GetMesh()->GetAnimInstance());
	// AIController 할당
	ai = Cast<AAIController>(me->GetController());

	//target 찾기
	/*
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVR_Player::StaticClass(), actors);
	for (auto tgt : actors)
	{
		target = Cast<AVR_Player>(tgt);
		break;
	}*/
}

// Called every frame
void UEnemyA_FSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (target == nullptr)
	{
		target = Cast<AVR_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), AVR_Player::StaticClass()));
	}

	switch (m_state_A)
	{
	case EEnemyAState::Idle:
		IdleState();
		break;
	case EEnemyAState::Move:
		MoveState();
		break;
	case EEnemyAState::Run:
		RunState();
		break;
	case EEnemyAState::Attack:
		AttackState();
		break;
	case EEnemyAState::Damage:
		DamageState();
		break;
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
	if (target == nullptr)
	{
		return;
	}

	me->GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	// Target인 Player 방향으로 이동
	// 	방향이 필요
	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	// 둘사이의 거리
	float distance = dir.Size();
	dir.Normalize();
	
	//AIController를 이용해서 이동
	if (ai)
	{
		ai->MoveToActor(target);
		anim->isMoving = true;
	}
	
	// character movement로 사용해서 따라가게 만들기
	//me->AddMovementInput(dir, true);

	// 타겟 방향으로 회전한다.		
	FRotator targetRot = dir.ToOrientationRotator();
	FRotator myRot = me->GetActorRotation();

	//회전 부드럽게
	myRot = FMath::Lerp(myRot, targetRot, 5 * GetWorld()->DeltaTimeSeconds);
	//me->SetActorRotation(myRot);

	//Debug Sphere 시각화
	//DrawDebugSphere(GetWorld(), me->GetActorLocation(), attackRange, 8, FColor::Red);

	// Timing
	currentTime += GetWorld()->DeltaTimeSeconds;

	// 속도가 있을 때, AnimInstance Bool 변경
	//if (anim->isMoving == false)
	//{
	//	//속도 구하기
	//	float velocity = me->GetVelocity().Size();
	//	if (velocity > 0.1f)
	//	{
	//		anim->isMoving = true;
	//	}
	//}
	// 피격을 받으면 bCAnHit이 true로 변경
	// 피격을 받으면 RUN state로 이동
	if (bCanHit == true && currentTime > RunDelayTime)
	{
		m_state_A = EEnemyAState::Run;
		anim->isMoving = false;
		anim->isRunning = true;
		currentTime = 0;
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

void UEnemyA_FSM::RunState()
{
	if (target == nullptr)
	{
		return;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("RUNNING!!")));
	me->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	// Target인 Player 방향으로 이동
	// 	방향이 필요
	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	// 둘사이의 거리
	float distance = dir.Size();
	dir.Normalize();

	//AIController를 이용해서 이동
	if (ai)
	{
		ai->MoveToActor(target);
		anim->isMoving = false;
		anim->isRunning = true;
	}

	// character movement로 사용해서 따라가게 만들기
	//me->AddMovementInput(dir, true);

	// 타겟 방향으로 회전한다.		
	FRotator targetRot = dir.ToOrientationRotator();
	FRotator myRot = me->GetActorRotation();

	//회전 부드럽게
	myRot = FMath::Lerp(myRot, targetRot, 5 * GetWorld()->DeltaTimeSeconds);
	//me->SetActorRotation(myRot);

	//Debug Sphere 시각화
	DrawDebugSphere(GetWorld(), me->GetActorLocation(), attackRange, 8, FColor::Red);
	// 공격 범위에 가까워지면

	if (distance < attackRange)
	{
		m_state_A = EEnemyAState::Attack;
		anim->isMoving = false;
		anim->isRunning = false;
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
	//EnemyA 어택 상태가 true 때만 Player 피격 판정 유효
	me->bCanAttack = true;

	// 시간이 흐른다.
	currentTime += GetWorld()->DeltaTimeSeconds;


	// HP = 0, isDie가 False라면
	if (Health == 0 && anim->isDie == false)
	{
		m_state_A = EEnemyAState::Die;
		currentTime = 0;
	}
}

void UEnemyA_FSM::DamageState()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Damaged!!")));

	anim->isRunning = false;
	anim->isDamaging = true;

	// Lerp로 knock back 구현
	FVector myPos = me->GetActorLocation();
	//knockbackPos 는 OnDamageProcess에서 계산
	myPos = FMath::Lerp(myPos, knockbackPos, 10 * GetWorld()->DeltaTimeSeconds);
	me->SetActorLocation(myPos);

	//거리 측정
	float distance = FVector::Dist(myPos, knockbackPos);

	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > 1.3f)
	{
		anim->isRunning = true;
		anim->isDamaging = false;
		m_state_A = EEnemyAState::Run;
		currentTime = 0;
	}
}

void UEnemyA_FSM::DieState()
{
	me->bCanAttack = false;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Dead!!")));
	anim->isMoving = false;
	anim->isAttacking = false;
	anim->isDie = true;

	if (bCanDie == true)
	{
		GetWorld()->GetTimerManager().SetTimer(DieTimerHandle, this, &UEnemyA_FSM::Die, 2.5f, false);
		bCanDie = false;
	}
}

void UEnemyA_FSM::OnDamageProcess(float damage, FVector KBDirection, bool isHead)
{
	if (Health > 0)
	{
		if (bCanHit == false)
		{
			bCanHit = true;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("bCanHit In!!")));
			return;
		}
		Health -= damage;
		if (Health < 0)
		{
			m_state_A = EEnemyAState::Die;
			//me->Destroy();
			return;
		}

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("KnockBack!!")));

		// Z 방향은 0으로 고정
		KBDirection.Z = 0;
		// 맞으면 뒤로 넉백
		knockbackPos = me->GetActorLocation() + KBDirection * knockback;

		// 상태를 Damage 로 이동
		m_state_A = EEnemyAState::Damage;
	}
}

void UEnemyA_FSM::Die()
{
	me->Destroy();
}
