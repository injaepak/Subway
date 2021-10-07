// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "FPSPlayer.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");

	WeaponName = "Default";
	BaseDamage = 100;
	WeaponMaxAmmo = 255; // 비사용중
	MagazineMaxAmmo = 30;

	CurrentTotalAmmo = WeaponMaxAmmo; // 비사용중
	CurrentMagazineAmmo = MagazineMaxAmmo;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}
void AWeaponBase::Fire()
{
}

void AWeaponBase::Reload()
{
}

//TArray<int32> AWeaponBase::GetCurrentAmmo()
//{
//	return { CurrentMagazineAmmo, CurrentTotalAmmo };
//}

