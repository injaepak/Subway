// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnMananger1.h"
#include "EnemyA.h"
#include "EnemyB.h"
#include "SpawnPoint.h"

#include <Kismet/GameplayStatics.h>

// Sets default values
ASpawnMananger1::ASpawnMananger1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnMananger1::BeginPlay()
{
	Super::BeginPlay();
	
	spawnPoint = Cast<ASpawnPoint>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpawnPoint::StaticClass()));
}

// Called every frame
void ASpawnMananger1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentTime += DeltaTime;

	if (check == 1)
	{
		if (currentTime >= 10.f)
		{
			GetWorld()->SpawnActor<AEnemyB>(bEnemyFactory, spawnPoint->spawn5->GetComponentTransform());
			check = 2;
		}
	}

	if (check == 2)
	{
		if (currentTime >= 30.f)
		{
			GetWorld()->SpawnActor<AEnemyB>(bEnemyFactory, spawnPoint->spawn2->GetComponentTransform());
			check = 3;
		}
	}

	if (check == 3)
	{
		if (currentTime >= 50.f)
		{
			GetWorld()->SpawnActor<AEnemyB>(bEnemyFactory, spawnPoint->spawn6->GetComponentTransform());
			check = 4;
		}
	}

	if (check == 4)
	{
		if (currentTime >= 70.f)
		{
			GetWorld()->SpawnActor<AEnemyB>(bEnemyFactory, spawnPoint->spawn4->GetComponentTransform());
			check = 5;
		}
	}

	if (check == 5)
	{
		if (currentTime >= 80.f)
		{
			GetWorld()->SpawnActor<AEnemyB>(bEnemyFactory, spawnPoint->spawn12->GetComponentTransform());
			check = 6;
		}
	}

	if (check == 6)
	{
		if (currentTime >= 90.f)
		{
			GetWorld()->SpawnActor<AEnemyB>(bEnemyFactory, spawnPoint->spawn7->GetComponentTransform());
			check = 7;
		}
	}

	if (check == 7)
	{
		if (currentTime >= 100.f)
		{
			GetWorld()->SpawnActor<AEnemyB>(bEnemyFactory, spawnPoint->spawn12->GetComponentTransform());
			check = 8;
		}
	}

	if (check == 8)
	{
		if (currentTime >= 107.f)
		{
			GetWorld()->SpawnActor<AEnemyA>(aEnemyFactory, spawnPoint->spawn10->GetComponentTransform());
			check = 9;
		}
	}

	if (check == 9)
	{
		if (currentTime >= 117.f)
		{
			GetWorld()->SpawnActor<AEnemyB>(bEnemyFactory, spawnPoint->spawn9->GetComponentTransform());
			check = 10;
		}
	}

	if (check == 10)
	{
		if (currentTime >= 127.f)
		{
			GetWorld()->SpawnActor<AEnemyA>(aEnemyFactory, spawnPoint->spawn12->GetComponentTransform());
			check = 11;
		}
	}

	if (check == 11)
	{
		if (currentTime >= 130.f)
		{
			GetWorld()->SpawnActor<AEnemyA>(aEnemyFactory, spawnPoint->spawn9->GetComponentTransform());
			check = 12;
		}
	}

	if (check == 12)
	{
		if (currentTime >= 135.f)
		{
			GetWorld()->SpawnActor<AEnemyA>(aEnemyFactory, spawnPoint->spawn2->GetComponentTransform());
			check = 13;

			
		}
	}

	// Fast Enemy
	if (check == 13)
	{
		if (currentTime >= 156.f)
		{
			GetWorld()->SpawnActor<AEnemyB>(bEnemyFactory, spawnPoint->spawn11->GetComponentTransform());
			check = 14;
		}
	}
	
	if (check == 14)
	{
		if (currentTime >= 167.f)
		{
			GetWorld()->SpawnActor<AEnemyB>(bEnemyFactory, spawnPoint->spawn5->GetComponentTransform());
			check = 15;
		}
	}

	if (check == 15)
	{
		if (currentTime >= 173.f)
		{
			GetWorld()->SpawnActor<AEnemyA>(aEnemyFactory, spawnPoint->spawn12->GetComponentTransform());
			check = 16;
		}
	}

	if (check == 16)
	{
		if(currentTime >= 174.f)
		{
			GetWorld()->SpawnActor<AEnemyA>(aEnemyFactory, spawnPoint->spawn1->GetComponentTransform());
			check = 17;
		}
	}

	if (check == 17)
	{
		if (currentTime >= 175.f)
		{
			GetWorld()->SpawnActor<AEnemyA>(aEnemyFactory, spawnPoint->spawn4->GetComponentTransform());
			check = 18;
		}
	}

	if (check == 18)
	{
		if (currentTime >= 185.f)
		{
			GetWorld()->SpawnActor<AEnemyB>(bEnemyFactory, spawnPoint->spawn1->GetComponentTransform());
			check = 19;
		}
	}

	if (check == 19)
	{
		if (currentTime >= 187.f)
		{
			GetWorld()->SpawnActor<AEnemyA>(aEnemyFactory, spawnPoint->spawn3->GetComponentTransform());
			check = 20;
		}
	}

	/*if (check == 20)
	{
		if (currentTime >= 204.f)
		{
			GetWorld()->SpawnActor<AEnemyB>(bEnemyFactory, spawnPoint->spawn6->GetComponentTransform());
			check = 21;
		}
	}*/

	// Fast Enemy
	if (check == 21)
	{
		if (currentTime >= 215.f)
		{
			GetWorld()->SpawnActor<AEnemyB>(bEnemyFactory, spawnPoint->spawn6->GetComponentTransform());
			check = 22;
		}
	}

	// Fast Enemy
	if (check == 22)
	{
		if (currentTime >= 226.f)
		{
			GetWorld()->SpawnActor<AEnemyB>(bEnemyFactory, spawnPoint->spawn9->GetComponentTransform());
			check = 23;
		}
	}

	if (check == 23)
	{
		if (currentTime >= 231.f)
		{
			GetWorld()->SpawnActor<AEnemyA>(aEnemyFactory, spawnPoint->spawn9->GetComponentTransform());
			check = 24;
		}
	}

	if (check == 24)
	{
		if (currentTime >= 241.f)
		{
			GetWorld()->SpawnActor<AEnemyA>(aEnemyFactory, spawnPoint->spawn7->GetComponentTransform());
			check = 25;
		}
	}

	if (check == 25)
	{
		if (currentTime >= 248.f)
		{
			GetWorld()->SpawnActor<AEnemyB>(bEnemyFactory, spawnPoint->spawn8->GetComponentTransform());
			check = 26;
		}
	}

	if (check == 26)
	{
		if (currentTime >= 251.f)
		{
			GetWorld()->SpawnActor<AEnemyB>(bEnemyFactory, spawnPoint->spawn9->GetComponentTransform());
			check = 27;
		}
	}

	if (check == 27)
	{
		if (currentTime >= 275.f)
		{
			GetWorld()->SpawnActor<AEnemyA>(aEnemyFactory, spawnPoint->spawn1->GetComponentTransform());
			check = 28;
		}
	}

	if (check == 28)
	{
		if (currentTime >= 285.f)
		{
			GetWorld()->SpawnActor<AEnemyA>(aEnemyFactory, spawnPoint->spawn5->GetComponentTransform());
			check = 29;
		}
	}

	if (check == 29)
	{
		if (currentTime >= 295.f)
		{
			GetWorld()->SpawnActor<AEnemyB>(bEnemyFactory, spawnPoint->spawn6->GetComponentTransform());
			check = 30;
		}
	}

	//// Fast Enemy
	//if (check == 30)
	//{
	//	if (currentTime >= 303.f)
	//	{
	//		GetWorld()->SpawnActor<AEnemyA>(aEnemyFactory, spawnPoint->spawn6->GetComponentTransform());
	//		check = 31;
	//	}
	//}

	if (check == 31)
	{
		if (currentTime >= 304.f)
		{
			GetWorld()->SpawnActor<AEnemyA>(aEnemyFactory, spawnPoint->spawn8->GetComponentTransform());
			check = 32;
		}
	}
}

