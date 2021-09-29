// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorOpenActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ADoorOpenActor::ADoorOpenActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	LDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LMesh"));
	RootComponent = LDoorMesh;
	//RDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RMesh"));
	//LDoorMesh->SetupAttachment(MainMesh);
	Open = false;
}

// Called when the game starts or when spawned
void ADoorOpenActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoorOpenActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DoorRotation = LDoorMesh->GetRelativeRotation();

	if (Open)
	{
		LDoorMesh->SetRelativeRotation(FMath::Lerp(FQuat(DoorRotation), FQuat(FRotator(0.0f, 0.0f, 0.0f)), 0.02f));
	}
}

void ADoorOpenActor::OnDamageProcess()
{
	Open = true;
	OpenDoor = true;

	FVector PawnLocation = this->GetActorLocation();
	FVector Direction = GetActorLocation() - PawnLocation;
	Direction = UKismetMathLibrary::LessLess_VectorRotator(Direction, GetActorRotation());

	if (Direction.X > 0.0f)
	{
		RotateValue = -90.0f;
	}
	else
	{
		RotateValue = 90.0f;
	}
}
