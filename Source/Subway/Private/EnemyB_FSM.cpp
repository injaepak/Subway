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
#include "AIController.h"
#include "NavigationSystem.h"
#include "NavigationInvokerComponent.h"

// Sets default values for this component's properties
UEnemyB_FSM::UEnemyB_FSM()
{
	PrimaryComponentTick.bCanEverTick = true;
	bCanDie = true;
}


// Called when the game starts
void UEnemyB_FSM::BeginPlay()
{
	Super::BeginPlay();
	// EnemyB = me
	me = Cast<AEnemyB>(GetOwner());
	anim = Cast<UEnemyBAnimInstance>(me->GetMesh()->GetAnimInstance());
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
void UEnemyB_FSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (target == nullptr)
	{
		target = Cast<AFPSPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AFPSPlayer::StaticClass()));
	}

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
	case EEnemyBState::Damage:
		DamageState();
		break;
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
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Move STATE!!")));
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
	//DrawDebugSphere(GetWorld(), me->GetActorLocation(), attackRange, 16, FColor::Red);

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
	//EnemyB 어택 상태가 true 때만 Player 피격 판정 유효
	me->bCanAttack = true;
	// 시간이 흐른다.
	currentTime += GetWorld()->DeltaTimeSeconds;

	// HP = 0, isDie가 False라면
	if (Health == 0 && anim->isDie == false)
	{
		m_state_B = EEnemyBState::Die;
		currentTime = 0;
	}
}

void UEnemyB_FSM::DamageState()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("DAMAGE STATE!!")));
	anim->isMoving = false;
	//anim->isDamaging = true;

	// Lerp 를 이용하여 knock back 구현
	FVector myPos = me->GetActorLocation();
	//knockbackPos 는 OnDamageProcess에서 계산
	myPos = FMath::Lerp(myPos, knockbackPos, 10 * GetWorld()->DeltaTimeSeconds);
	me->SetActorLocation(myPos);

	//거리 측정
	float distance = FVector::Dist(myPos, knockbackPos);

	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > 1.2f)
	{
		anim->isMoving = true;
		anim->isHead = false;
		anim->isBody = false;
		m_state_B = EEnemyBState::Move;
		currentTime = 0;
	}
	// HP = 0, isDie가 False라면
	else if (Health == 0 && anim->isDie == false)
	{
		m_state_B = EEnemyBState::Die;
		currentTime = 0;
	}

	// 최종 위치와 나와의 거리가 아주 작으면 Attack으로 상태 변경
	/*if (distance < 95.f)
	{
		myPos = knockbackPos;
		m_state_B = EEnemyBState::Attack;
		anim->isDamaging = false;
		anim->isAttacking = true;
		currentTime = 0;
	}*/

}

void UEnemyB_FSM::DieState()
{
	me->bCanAttack = false;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Dead!!")));
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

void UEnemyB_FSM::OnDamageProcess(float damage, FVector KBDirection, bool isHead)
{
	if (Health > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("KnockBack!!")));

		// Z 방향은 0으로 고정
		KBDirection.Z = 0;
		// 맞으면 뒤로 넉백
		knockbackPos = me->GetActorLocation() + KBDirection * knockback;

		// 상태를 Damage 로 이동
		m_state_B = EEnemyBState::Damage;

		if (isHead == true)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("HEADSHOT!!")));
			anim->isHead = true;
			anim->isBody = false;
		}
		else if (isHead == false)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("BODYSHOT!!")));
			anim->isHead = false;
			anim->isBody = true;
		}

		// 데미지 처리
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

