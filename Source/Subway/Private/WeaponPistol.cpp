// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPistol.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AWeaponPistol::AWeaponPistol()
{
	WeaponName = "Pistol";
	BaseDamage = 100;
	WeaponMaxAmmo = 98;
	MagazineMaxAmmo = 7;

	CurrentTotalAmmo = WeaponMaxAmmo;
	CurrentMagazineAmmo = MagazineMaxAmmo;
}

void AWeaponPistol::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("SHOOTING PISTOL"));

	FVector Start = WeaponMesh->GetBoneLocation(FName("b_gun_muzzleflash"));
	FVector Rot = WeaponMesh->GetBoneQuaternion(FName("b_gun_muzzleflash")).Vector();
	FVector End = Start + Rot * 2000.0f;

	TArray<FHitResult> HitResults;
	FCollisionQueryParams CollisionParams;
	FCollisionResponseParams CollisionResponse;
	CollisionParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceMultiByChannel(OUT HitResults, Start, End, ECollisionChannel::ECC_GameTraceChannel12, CollisionParams, CollisionResponse))
	{
		DrawDebugLine(GetWorld(), Start, End, FColor(255, 0, 0), false, 0.f, 0.f, 10.f);
		for (FHitResult& Result : HitResults)
		{
			if (AActor* HitActor = Result.GetActor())
			{
				UE_LOG(LogTemp, Warning, TEXT("Actor Name: %s"), *HitActor->GetName());
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("You are hitting: %s"), *HitActor->GetName()));
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Cause Damage: %d"), BaseDamage));
			}
		}
	}
}

void AWeaponPistol::Reload()
{

}