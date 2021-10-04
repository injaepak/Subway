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
	// AIController �Ҵ�
	ai = Cast<AAIController>(me->GetController());

	//target ã��
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
	if (target == nullptr)
	{
		return;
	}

	me->GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	// Target�� Player �������� �̵�
	// 	������ �ʿ�
	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	// �ѻ����� �Ÿ�
	float distance = dir.Size();
	dir.Normalize();
	
	//AIController�� �̿��ؼ� �̵�
	if (ai)
	{
		ai->MoveToActor(target);
		anim->isMoving = true;
	}
	
	// character movement�� ����ؼ� ���󰡰� �����
	//me->AddMovementInput(dir, true);

	// Ÿ�� �������� ȸ���Ѵ�.		
	FRotator targetRot = dir.ToOrientationRotator();
	FRotator myRot = me->GetActorRotation();

	//ȸ�� �ε巴��
	myRot = FMath::Lerp(myRot, targetRot, 5 * GetWorld()->DeltaTimeSeconds);
	//me->SetActorRotation(myRot);

	//Debug Sphere �ð�ȭ
	//DrawDebugSphere(GetWorld(), me->GetActorLocation(), attackRange, 8, FColor::Red);

	// Timing
	currentTime += GetWorld()->DeltaTimeSeconds;

	// �ӵ��� ���� ��, AnimInstance Bool ����
	//if (anim->isMoving == false)
	//{
	//	//�ӵ� ���ϱ�
	//	float velocity = me->GetVelocity().Size();
	//	if (velocity > 0.1f)
	//	{
	//		anim->isMoving = true;
	//	}
	//}
	// �ǰ��� ������ bCAnHit�� true�� ����
	// �ǰ��� ������ RUN state�� �̵�
	if (bCanHit == true && currentTime > RunDelayTime)
	{
		m_state_A = EEnemyAState::Run;
		anim->isMoving = false;
		anim->isRunning = true;
		currentTime = 0;
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

void UEnemyA_FSM::RunState()
{
	if (target == nullptr)
	{
		return;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("RUNNING!!")));
	me->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	// Target�� Player �������� �̵�
	// 	������ �ʿ�
	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	// �ѻ����� �Ÿ�
	float distance = dir.Size();
	dir.Normalize();

	//AIController�� �̿��ؼ� �̵�
	if (ai)
	{
		ai->MoveToActor(target);
		anim->isMoving = false;
		anim->isRunning = true;
	}

	// character movement�� ����ؼ� ���󰡰� �����
	//me->AddMovementInput(dir, true);

	// Ÿ�� �������� ȸ���Ѵ�.		
	FRotator targetRot = dir.ToOrientationRotator();
	FRotator myRot = me->GetActorRotation();

	//ȸ�� �ε巴��
	myRot = FMath::Lerp(myRot, targetRot, 5 * GetWorld()->DeltaTimeSeconds);
	//me->SetActorRotation(myRot);

	//Debug Sphere �ð�ȭ
	DrawDebugSphere(GetWorld(), me->GetActorLocation(), attackRange, 8, FColor::Red);
	// ���� ������ ���������

	if (distance < attackRange)
	{
		m_state_A = EEnemyAState::Attack;
		anim->isMoving = false;
		anim->isRunning = false;
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
	//EnemyA ���� ���°� true ���� Player �ǰ� ���� ��ȿ
	me->bCanAttack = true;

	// �ð��� �帥��.
	currentTime += GetWorld()->DeltaTimeSeconds;


	// HP = 0, isDie�� False���
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

	// Lerp�� knock back ����
	FVector myPos = me->GetActorLocation();
	//knockbackPos �� OnDamageProcess���� ���
	myPos = FMath::Lerp(myPos, knockbackPos, 10 * GetWorld()->DeltaTimeSeconds);
	me->SetActorLocation(myPos);

	//�Ÿ� ����
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

		// Z ������ 0���� ����
		KBDirection.Z = 0;
		// ������ �ڷ� �˹�
		knockbackPos = me->GetActorLocation() + KBDirection * knockback;

		// ���¸� Damage �� �̵�
		m_state_A = EEnemyAState::Damage;
	}
}

void UEnemyA_FSM::Die()
{
	me->Destroy();
}
