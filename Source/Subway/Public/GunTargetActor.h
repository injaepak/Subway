// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunTargetActor.generated.h"

UCLASS()
class SUBWAY_API AGunTargetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunTargetActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// StaticMesh
	UPROPERTY(VisibleAnywhere, Category = "TargetMesh")
	class UBoxComponent* Collision;
	UPROPERTY(VisibleAnywhere, Category = "TargetMesh")
	class UStaticMeshComponent* Mesh;

	//½Ã°£
	UPROPERTY(EditAnywhere, Category = FSM)
	float returnDelayTime = 4;
	UPROPERTY()
	float currentTime = 0;

	bool bHit;

	void OnDamageProcess();

	// declare our float variables 	
	UPROPERTY(EditAnywhere, Category = Movement)
	float PitchValue;

	UPROPERTY(EditAnywhere, Category = Movement)
	float YawValue;

	UPROPERTY(EditAnywhere, Category = Movement)
	float RollValue;
};
