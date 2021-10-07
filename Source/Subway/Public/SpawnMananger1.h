// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnMananger1.generated.h"

UCLASS()
class SUBWAY_API ASpawnMananger1 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnMananger1();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyFactory")
		TSubclassOf<class AEnemyA> aEnemyFactory;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyFactory")
		TSubclassOf<class AEnemyB> bEnemyFactory;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyFactory")
		TSubclassOf<class ABoss> bossEnemyFactory;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyFactory")
		class UParticleSystem* particle;

	UPROPERTY(EditAnywhere, Category = "SpawnPoint")
		class ASpawnPoint* spawnPoint;

	UPROPERTY()
	float currentTime = 0;

	UPROPERTY()
	int32 check = 1;
};
