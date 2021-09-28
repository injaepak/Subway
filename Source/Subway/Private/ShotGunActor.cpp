// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotGunActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ChildActorComponent.h"

// Sets default values
AShotGunActor::AShotGunActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(boxComp);

	// 물리 관련 설정
	boxComp->SetSimulatePhysics(true);
	boxComp->SetEnableGravity(true);
	boxComp->SetCollisionProfileName(TEXT("PickUP"));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(boxComp);

	shotgun = CreateDefaultSubobject<UChildActorComponent>(TEXT("Shootgun"));
	shotgun->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AShotGunActor::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
//void AShotGunActor::Tick(float DeltaTime)
//{
	//Super::Tick(DeltaTime);

//}

