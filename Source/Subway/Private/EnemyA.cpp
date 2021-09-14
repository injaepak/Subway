// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyA.h"
#include "EnemyA_FSM.h"
#include "Engine/Engine.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Animation/AnimInstance.h"

// Sets default values
AEnemyA::AEnemyA()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	enemyAFSM = CreateDefaultSubobject<UEnemyA_FSM>(TEXT("EnemyAFSM"));

	// BossMesh 붙이기
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Model/derrick.derrick'"));
	
	// Skeletal Mesh Load 성공하면, 데이터 할당
	if (tempMesh.Succeeded())
	{
		// 데이터
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
	}

	//Anime BP 할당
	ConstructorHelpers::FClassFinder<UAnimInstance>tempAnim(TEXT("AnimBlueprint'/Game/Animation/ABP_EnemyA.ABP_EnemyA_C'"));
	
	if (tempAnim.Succeeded())
	{
		// 데이터
		GetMesh()->SetAnimClass(tempAnim.Class);
	}

	//Health System
	bCanBeDamaged = true;
	/*EnemyAHealth = 0.f;
	EnemyAMaxHealth = 1000.0f;
	EnemyAHealth = EnemyAMaxHealth;
	EnemyAHealthPercentage = 1.0f;*/

}

// Called when the game starts or when spawned
void AEnemyA::BeginPlay()
{
	Super::BeginPlay();
	
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

