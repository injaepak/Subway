// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSPlayer.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class SUBWAY_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponBase();
	
protected:
	UPROPERTY(EditAnywhere, Category = "Test")
		class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, Category = "Test")
		class UAnimSequence* FireAnimation;

	UPROPERTY(EditAnywhere, Category = "Test")
		class UAnimationAsset* ReloadAnimation;

	UPROPERTY(EditAnywhere, Category = "Test")
		FString WeaponName;

	UPROPERTY(EditAnywhere, Category = "Test")
		int32 BaseDamage;

	UPROPERTY(EditAnywhere, Category = "Test")
		int32 WeaponMaxAmmo;

	UPROPERTY(EditAnywhere, Category = "Test")
		int32 CurrentTotalAmmo;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Fire();
	virtual void Reload();
	TArray<int32> GetCurrentAmmo();
	/*UPROPERTY()
		class UEnemyAAnimInstance* anim;*/

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
		int32 CurrentMagazineAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
		int32 MagazineMaxAmmo;

	UPROPERTY(EditDefaultsOnly, Category = "Test")
		class UParticleSystem* bulletShootEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Test")
		class UParticleSystem* bulletEnemyHitEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Test")
		class UParticleSystem* bulletAnotherHitEffect;
};
