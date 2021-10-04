// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyB.h"
#include "EnemyB_FSM.h"
#include "Engine/Engine.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Animation/AnimInstance.h"
#include "FPSPlayer.h"
#include "Components/BoxComponent.h"

// Sets default values
AEnemyB::AEnemyB()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	enemyBFSM = CreateDefaultSubobject<UEnemyB_FSM>(TEXT("EnemyBFSM"));

	//CollisionComponent Attachment
	HeadCollision = CreateDefaultSubobject<USphereComponent>(TEXT("HeadCollision"));
	HeadCollision->SetupAttachment(GetMesh(), "HeadTarget");

	BodyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BodyCollision->SetupAttachment(GetMesh(), "Spine1");

	RtCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RtHandCollision"));
	RtCollision->SetupAttachment(GetMesh(), "RightHandTarget");

	LtCollision = CreateDefaultSubobject<USphereComponent>(TEXT("LtHandCollision"));
	LtCollision->SetupAttachment(GetMesh(), "LeftHandTarget");
	LtCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyB::OnOverlapBegin);

	// BossMesh ���̱�
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Model/EnemyB/ZombieB_jill.ZombieB_jill'"));

	// Skeletal Mesh Load �����ϸ�, ������ �Ҵ�
	if (tempMesh.Succeeded())
	{
		// ������
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
	}

	//Anime BP �Ҵ�
	ConstructorHelpers::FClassFinder<UAnimInstance>tempAnim(TEXT("AnimBlueprint'/Game/Animation/ABP_EnemyB.ABP_EnemyB_C'"));

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
void AEnemyB::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), zombieSound, GetActorLocation(), FRotator::ZeroRotator, 1.f, 1.f, 0.0f, nullptr, nullptr);
}

// Called every frame
void AEnemyB::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyB::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyB::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Player�� OtherActor�� ����
	auto Player = Cast<AFPSPlayer>(OtherActor);
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
			GetWorld()->GetTimerManager().SetTimer(EnemyBOverlapTimerHandle, this, &AEnemyB::ResetOverlapTimer, 2.5f, false);
		}
	}
}

void AEnemyB::ResetOverlapTimer()
{
	bCanOverlap = true;
	GetWorld()->GetTimerManager().ClearTimer(EnemyBOverlapTimerHandle);
}

