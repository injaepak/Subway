// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorOpenActor.generated.h"

UCLASS()
class SUBWAY_API ADoorOpenActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorOpenActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* MainMesh;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* LDoorMesh;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* RDoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Test)
	bool OpenDoor = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Test)
	bool Open;

	float RotateValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Rotator)
	FRotator DoorRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Rotator)
	FRotator	LDoorRotation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Rotator)
	FRotator	RDoorRotation;


	void OnDamageProcess();
};
