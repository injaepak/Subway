// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyB_FSM.h"
#include "GameFramework/Character.h"
#include "EnemyB.generated.h"

UCLASS()
class SUBWAY_API AEnemyB : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyB();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, Category = EnemyBFSM, BlueprintReadWrite)
	class UEnemyB_FSM* enemyBFSM;
	
	UPROPERTY(VisibleAnywhere, Category = Collision)
	class USphereComponent* HeadCollision;
	UPROPERTY(VisibleAnywhere, Category = Collision)
	class UBoxComponent* BodyCollision;
	UPROPERTY(VisibleAnywhere, Category = Collision)
	class USphereComponent* RtCollision;
	UPROPERTY(VisibleAnywhere, Category = Collision)
	class USphereComponent* LtCollision;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// Health System
	bool bCanBeDamaged;
	bool bCanAttack;

	bool bCanOverlap;
	void ResetOverlapTimer();
	FTimerHandle EnemyBOverlapTimerHandle;
};
