// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyB.h"
#include "EnemyB_FSM.h"
#include "Engine/Engine.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Animation/AnimInstance.h"

// Sets default values
AEnemyB::AEnemyB()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	enemyBFSM = CreateDefaultSubobject<UEnemyB_FSM>(TEXT("EnemyBFSM"));

	// BossMesh 붙이기
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Model/EnemyB/ZombieB_jill.ZombieB_jill'"));
	
	// Skeletal Mesh Load 성공하면, 데이터 할당
	if (tempMesh.Succeeded())
	{
		// 데이터
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
	}

	//Anime BP 할당
	ConstructorHelpers::FClassFinder<UAnimInstance>tempAnim(TEXT("AnimBlueprint'/Game/Animation/ABP_EnemyB.ABP_EnemyB_C'"));
	
	if (tempAnim.Succeeded())
	{
		// 데이터
		GetMesh()->SetAnimClass(tempAnim.Class);
	}

	//Health System
	bCanBeDamaged = true;
}

// Called when the game starts or when spawned
void AEnemyB::BeginPlay()
{
	Super::BeginPlay();
	
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

