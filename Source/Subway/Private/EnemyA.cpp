// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyA.h"
#include "EnemyA_FSM.h"
#include "Engine/Engine.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Animation/AnimInstance.h"
#include "FPSPlayer.h"
#include "Components/BoxComponent.h"

// Sets default values
AEnemyA::AEnemyA()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	enemyAFSM = CreateDefaultSubobject<UEnemyA_FSM>(TEXT("EnemyAFSM"));

	//CollisionComponent Attachment
	HeadCollision = CreateDefaultSubobject<USphereComponent>(TEXT("HeadCollision"));
	HeadCollision->SetupAttachment(GetMesh(), "HeadTarget");

	BodyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BodyCollision->SetupAttachment(GetMesh(), "Spine1");

	RtCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RtHandCollision"));
	RtCollision->SetupAttachment(GetMesh(), "RightHandTarget");
	RtCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyA::OnOverlapBegin);

	LtCollision = CreateDefaultSubobject<USphereComponent>(TEXT("LtHandCollision"));
	LtCollision->SetupAttachment(GetMesh(), "LeftHandTarget");

	// BossMesh ���̱�
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Model/derrick.derrick'"));

	// Skeletal Mesh Load �����ϸ�, ������ �Ҵ�
	if (tempMesh.Succeeded())
	{
		// ������
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
	}

	//Anime BP �Ҵ�
	ConstructorHelpers::FClassFinder<UAnimInstance>tempAnim(TEXT("AnimBlueprint'/Game/Animation/ABP_EnemyA.ABP_EnemyA_C'"));

	if (tempAnim.Succeeded())
	{
		// ������
		GetMesh()->SetAnimClass(tempAnim.Class);
	}

	//Health System
	bCanBeDamaged = true;
	bCanOverlap = true;
	bCanAttack = false;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AEnemyA::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), zombieSound, GetActorLocation(), FRotator::ZeroRotator, 1.f, 1.f, 0.0f, nullptr, nullptr);
}

// Called every frame
void AEnemyA::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyA::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyA::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Player�� OtherActor�� ����
	auto Player = Cast<AVR_Player>(OtherActor);
	// ����� Player�̰�, �ڽ��� �ƴ϶��
	if (Player && (OtherActor != this))
	{
		// bCanOverlap Boolean�� true���
		if (bCanOverlap && bCanAttack == true)
		{
			bCanOverlap = false;
			//�÷��̾��� OnDamageProcess�� �̵�
			Player->OnDamageProcess();
			//Ÿ�̸� ����
			GetWorld()->GetTimerManager().SetTimer(EnemyAOverlapTimerHandle, this, &AEnemyA::ResetOverlapTimer, 2.5f, false);
		}
	}
}

void AEnemyA::ResetOverlapTimer()
{
	bCanOverlap = true;
	GetWorld()->GetTimerManager().ClearTimer(EnemyAOverlapTimerHandle);
}

