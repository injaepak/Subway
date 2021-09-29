// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnPoint.h"

// Sets default values
ASpawnPoint::ASpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	spawn0 = CreateDefaultSubobject<USceneComponent>(TEXT("spawn0"));
	SetRootComponent(spawn0);
	spawn1 = CreateDefaultSubobject<USceneComponent>(TEXT("spawn1"));
	spawn1->SetupAttachment(RootComponent);
	spawn2 = CreateDefaultSubobject<USceneComponent>(TEXT("spawn2"));
	spawn2->SetupAttachment(RootComponent);
	spawn3 = CreateDefaultSubobject<USceneComponent>(TEXT("spawn3"));
	spawn3->SetupAttachment(RootComponent);
	spawn4 = CreateDefaultSubobject<USceneComponent>(TEXT("spawn4"));
	spawn4->SetupAttachment(RootComponent);
	spawn5 = CreateDefaultSubobject<USceneComponent>(TEXT("spawn5"));
	spawn5->SetupAttachment(RootComponent);
	spawn6 = CreateDefaultSubobject<USceneComponent>(TEXT("spawn6"));
	spawn6->SetupAttachment(RootComponent);
	spawn7 = CreateDefaultSubobject<USceneComponent>(TEXT("spawn7"));
	spawn7->SetupAttachment(RootComponent);
	spawn8 = CreateDefaultSubobject<USceneComponent>(TEXT("spawn8"));
	spawn8->SetupAttachment(RootComponent);
	spawn9 = CreateDefaultSubobject<USceneComponent>(TEXT("spawn9"));
	spawn9->SetupAttachment(RootComponent);
	spawn10 = CreateDefaultSubobject<USceneComponent>(TEXT("spawn10"));
	spawn10->SetupAttachment(RootComponent);
	spawn11 = CreateDefaultSubobject<USceneComponent>(TEXT("spawn11"));
	spawn11->SetupAttachment(RootComponent);
	spawn12 = CreateDefaultSubobject<USceneComponent>(TEXT("spawn12"));
	spawn12->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

