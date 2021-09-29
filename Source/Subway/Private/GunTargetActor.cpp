// Fill out your copyright notice in the Description page of Project Settings.


#include "GunTargetActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGunTargetActor::AGunTargetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Body Mesh ������Ʈ �߰�
	//Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	//RootComponent = Collision;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMesh"));
	RootComponent = Mesh;
	//Mesh->SetupAttachment(RootComponent);

	bHit = true;

	PitchValue = -90.f;
	YawValue = 0.f;
	RollValue = 0.f;
}

// Called when the game starts or when spawned
void AGunTargetActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGunTargetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (bHit == false && currentTime > returnDelayTime)
	{
		FQuat QuatRotation = FRotator(90.0f, 0.0f, 0.0f).Quaternion();
		AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
		//AddActorWorldRotation(QuatRotation, false, 0, ETeleportType::None);
		bHit = true;
		currentTime = 0;
	}
}

void AGunTargetActor::OnDamageProcess()
{
	bHit = false;
	FQuat QuatRotation = FRotator(PitchValue, YawValue, RollValue).Quaternion();
	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
	//AddActorWorldRotation(QuatRotation, false, 0, ETeleportType::None);
}

