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
	//target ã��
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
	// ���¸� Move�� �ٲ۴�.
	currentTime += GetWorld()->DeltaTimeSeconds;

	// �ð��� �������� ���� ����
	if (currentTime > idleDelayTime)
	{
		m_state_A = EEnemyAState::Move;
		// CurrentTime ����
		currentTime = 0;
	}
	// HP = 0, isDie�� False���
	else if (Health == 0 && anim->isDie == false)
	{
		m_state_A = EEnemyAState::Die;
		currentTime = 0;
	}
}

void UEnemyA_FSM::MoveState()
{
	me->GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	// Target�� Player �������� �̵�
	// 	������ �ʿ�
	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	// �ѻ����� �Ÿ�
	float distance = dir.Size();
	dir.Normalize();

	// character movement�� ����ؼ� ���󰡰� �����
	me->AddMovementInput(dir, true);

	// Ÿ�� �������� ȸ���Ѵ�.		
	FRotator targetRot = dir.ToOrientationRotator();
	FRotator myRot = me->GetActorRotation();

	//ȸ�� �ε巴��
	myRot = FMath::Lerp(myRot, targetRot, 5 * GetWorld()->DeltaTimeSeconds);
	me->SetActorRotation(myRot);

	//Debug Sphere �ð�ȭ
	DrawDebugSphere(GetWorld(), me->GetActorLocation(), attackRange, 8, FColor::Red);

	// �ӵ��� ���� ��, AnimInstance Bool ����
	if (anim->isMoving == false)
	{
		//�ӵ� ���ϱ�
		float velocity = me->GetVelocity().Size();
		if (velocity > 0.1f)
		{
			anim->isMoving = true;
		}
	}

	// state ���
	// ���� ������ ���������
	if (distance <= attackRange)
	{
		m_state_A = EEnemyAState::Attack;
		anim->isMoving = false;
		anim->isAttacking = true;
		currentTime = attackDelayTime;
	}

	// HP = 0, isDie�� False���
	if (Health == 0 && anim->isDie == false)
	{
		m_state_A = EEnemyAState::Die;
		currentTime = 0;
	}
}

void UEnemyA_FSM::AttackState()
{
	// �ð��� �帥��.
	currentTime += GetWorld()->DeltaTimeSeconds;

	// ���� �ð��� �������� �������� ����
	if (currentTime > attackDelayTime)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("ATTACK!!")));
		// �ð� �ʱ�ȭ
		currentTime = 0;
	}
	// HP = 0, isDie�� False���
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

	//���¸� DamageState�� �̵�
	//m_state_A = EEnemyAState::Damage;
	//currentTime = 0;
}

void UEnemyA_FSM::Die()
{
	me->Destroy();
}
