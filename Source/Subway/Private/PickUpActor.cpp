// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpActor.h"
#include "VR_Player.h"
#include <Kismet/GameplayStatics.h>
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ChildActorComponent.h"


// Sets default values
APickUpActor::APickUpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(boxComp);
	
	// 물리 관련 설정
	boxComp->SetSimulatePhysics(true);
	boxComp->SetEnableGravity(true);
	boxComp->SetCollisionProfileName(TEXT("PickUP"));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(boxComp);

	gun = CreateDefaultSubobject<UChildActorComponent>(TEXT("Gun"));
	gun->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APickUpActor::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVR_Player::StaticClass(), actors);

	for (auto tgt : actors)
	{
		player = Cast<AVR_Player>(tgt);
		break;
	}

}

 //Called every frame
void APickUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bGrip == false)
	{	
		/*FVector me = GetActorLocation();
		FVector dir = player->gunComp->GetComponentLocation() - me;
		dir.Normalize();

		FVector P = me + dir * DeltaTime * speed;


		SetActorLocation(P);*/
	}
}

