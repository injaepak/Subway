// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_FSM.h"
#include "Boss.h"
#include "FPSPlayer.h"
#include "VR_Player.h"
#include "BossAnimInstance.h"
#include "Subway.h"
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
UBoss_FSM::UBoss_FSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bCanDie = true;
}


// Called when the game starts
void UBoss_FSM::BeginPlay()
{
	Super::BeginPlay();
	// EnemyB = me
	me = Cast<ABoss>(GetOwner());
	anim = Cast<UBossAnimInstance>(me->GetMesh()->GetAnimInstance());
	ai = Cast<AAIController>(me->GetController());
}


// Called every frame
void UBoss_FSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (target == nullptr)
	{
		target = Cast<AFPSPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AFPSPlayer::StaticClass()));
	}

	switch (m_state_Boss)
	{
	case EBossState::Idle:
		IdleState();
		break;
	case EBossState::Trans:
		TransState();
		break;
	case EBossState::Taunt:
		TauntState();
		break;
	case EBossState::Move:
		MoveState();
		break;
	case EBossState::Attack:
		AttackState();
		break;
	case EBossState::Damage:
		DamageState();
		break;
	case EBossState::Die:
		DieState();
		break;
	}
}

void UBoss_FSM::IdleState()
{
	if (target == nullptr)
	{
		return;
	}

	currentTime += GetWorld()->DeltaTimeSeconds;

	// 시간이 지났으니 상태 변경
	if (currentTime > idleDelayTime)
	{
		m_state_Boss = EBossState::Trans;
		anim->isTrans = true;
		// CurrentTime 리셋
		currentTime = 0;
	}
}

void UBoss_FSM::TransState()
{
	if (target == nullptr)
	{
		return;
	}

	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > TransDelayTime)
	{
		anim->isTrans = false;
		anim->isTaunt = true;
		m_state_Boss = EBossState::Taunt;
		// CurrentTime 리셋
		currentTime = TauntDelayTime;
	}
}

void UBoss_FSM::TauntState()
{
	if (target == nullptr)
	{
		return;
	}

	currentTime += GetWorld()->DeltaTimeSeconds;
	// 	방향이 필요
	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	dir.Normalize();
	// 타겟 방향으로 회전한다.		
	FRotator targetRot = dir.ToOrientationRotator();
	FRotator myRot = me->GetActorRotation();
	//회전 부드럽게
	myRot = FMath::Lerp(myRot, targetRot, 5 * GetWorld()->DeltaTimeSeconds);
	me->SetActorRotation(myRot);

	if (currentTime > 5.7f)
	{
		anim->isTaunt = false;
		anim->isMoving = true;
		m_state_Boss = EBossState::Move;
		// CurrentTime 리셋
		currentTime = 0;
	}
}

void UBoss_FSM::MoveState()
{
	PRINTLOG(TEXT("MOVE"));
	if (target == nullptr)
	{
		return;
	}

	me->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	// Target인 Player 방향으로 이동
	// 	방향이 필요
	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	// 둘사이의 거리
	float distance = dir.Size();
	dir.Normalize();

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

	// 속도가 있을 때, AnimInstance Bool 변경
	/*
	if (anim->isMoving == false)
	{
		//속도 구하기
		float velocity = me->GetVelocity().Size();
		if (velocity > 0.1f)
		{
			anim->isMoving = true;
		}
	}*/

	DrawDebugSphere(GetWorld(), me->GetActorLocation(), attackRange, 16, FColor::Red);

	// state 경과
	// 공격 범위에 가까워지면
	if (distance < attackRange)
	{
		m_state_Boss = EBossState::Attack;
		anim->isMoving = false;
		anim->isAttacking = true;
		currentTime = attackDelayTime;
	}	

	// HP = 0, isDie가 False라면
	if (Health == 0 && anim->isDie == false)
	{
		m_state_Boss = EBossState::Die;
		currentTime = 0;
	}
}

void UBoss_FSM::AttackState()
{
	PRINTLOG(TEXT("ATTACK"));
	if (ai)
	{
		ai->StopMovement();
		anim->isMoving = false;
		anim->isAttacking = true;
	}
	me->bCanAttack = true;
	
	// 시간이 흐른다.
	currentTime += GetWorld()->DeltaTimeSeconds;

	// HP = 0, isDie가 False라면
	if (Health == 0 && anim->isDie == false)
	{
		m_state_Boss = EBossState::Die;
		currentTime = 0;
	}
}

void UBoss_FSM::DamageState()
{
	PRINTLOG(TEXT("DAMAGE"));

	anim->isMoving = false;
	anim->isDamage = true;
	
	// Lerp 를 이용하여 knock back 구현
	FVector myPos = me->GetActorLocation();
	//knockbackPos 는 OnDamageProcess에서 계산
	myPos = FMath::Lerp(myPos, knockbackPos, 10 * GetWorld()->DeltaTimeSeconds);
	me->SetActorLocation(myPos);

	//거리 측정
	float distance = FVector::Dist(myPos, knockbackPos);

	currentTime += GetWorld()->DeltaTimeSeconds;
	
	if (bhit == true && currentTime > 1.5f)
	{
		//bhit = false;
		anim->isDamage = false;
		anim->isMoving = true;
		m_state_Boss = EBossState::Move;
		currentTime = 0;
	}
	else if (m_state_Boss == EBossState::Attack && currentTime > 2.f)
	{
		anim->isDamage = false;
		anim->isAttacking = true;
		m_state_Boss = EBossState::Attack;
		currentTime = 0;
	}
	// HP = 0, isDie가 False라면
	else if (Health == 0 && anim->isDie == false)
	{
		m_state_Boss = EBossState::Die;
		currentTime = 0;
	}
}

void UBoss_FSM::DieState()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Dead!!")));
	anim->isMoving = false;
	anim->isAttacking = false;
	anim->isDie = true;

	if (bCanDie == true)
	{
		GetWorld()->GetTimerManager().SetTimer(DieTimerHandle, this, &UBoss_FSM::Die, 2.5f, false);
		bCanDie = false;
	}
}

void UBoss_FSM::OnDamageProcess(float damage, FVector KBDirection, bool isHead)
{
	if (Health > 0)
	{
		//isHeadPart와 isHead 변수 연결
		//this->isHeadPart = isHead;

		// Z 방향은 0으로 고정
		KBDirection.Z = 0;
		// 맞으면 뒤로 넉백
		knockbackPos = me->GetActorLocation() + KBDirection * knockback;
		currentTime = 0;

		bhit = true;
		m_state_Boss = EBossState::Damage;


		// 데미지 처리
		Health -= damage;
		if (Health < 0)
		{
			m_state_Boss = EBossState::Die;
			//me->Destroy();
			return;
		}
	}
}

void UBoss_FSM::Die()
{
	me->Destroy();
}

