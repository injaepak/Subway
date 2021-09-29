// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPoint.generated.h"

UCLASS()
class SUBWAY_API ASpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class USceneComponent* spawn0;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class USceneComponent* spawn1;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class USceneComponent* spawn2;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class USceneComponent* spawn3;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class USceneComponent* spawn4;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class USceneComponent* spawn5;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class USceneComponent* spawn6;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class USceneComponent* spawn7;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class USceneComponent* spawn8;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class USceneComponent* spawn9;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class USceneComponent* spawn10;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class USceneComponent* spawn11;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class USceneComponent* spawn12;
};
