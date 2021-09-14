// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponShotgun.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSPlayer.h"
#include "EnemyA.h"
#include "EnemyA_FSM.h"

// Sets default values
AWeaponShotgun::AWeaponShotgun()
{
	WeaponName = "ShotGun";
	BaseDamage = 200;
	WeaponMaxAmmo = 98;
	MagazineMaxAmmo = 7;

	CurrentTotalAmmo = WeaponMaxAmmo;
	CurrentMagazineAmmo = MagazineMaxAmmo;
}

void AWeaponShotgun::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("SHOOTING SHOTGUN"));
	FVector Start = WeaponMesh->GetBoneLocation(FName("b_gun_muzzleflash"));
	FVector Rot = WeaponMesh->GetBoneQuaternion(FName("b_gun_muzzleflash")).Vector();
	FVector End = Start + Rot * 5000.0f;

	//TArray<FHitResult> HitResults;
	FHitResult HitResults;
	FCollisionQueryParams CollisionParams;
	FCollisionResponseParams CollisionResponse;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResults, Start, End, ECollisionChannel::ECC_GameTraceChannel12, CollisionParams, CollisionResponse);

	if (bHit)
	{
		DrawDebugLine(GetWorld(), Start, End, FColor(255, 0, 0), false, 0.f, 0.f, 10.f);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("HIT!!")));
		/*
		for (FHitResult& Result : HitResults)
		{
			if (AActor* HitActor = Result.GetActor())
			{
				UE_LOG(LogTemp, Warning, TEXT("Actor Name: %s"), *HitActor->GetName());
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("You are hitting: %s"), *HitActor->GetName()));
			}
		}
		*/
		auto enemyA = Cast<AEnemyA>(HitResults.GetActor());
		if (enemyA)
		{
			enemyA->enemyAFSM->OnDamageProcess();
		}
	}
}

void AWeaponShotgun::Reload()
{

}

