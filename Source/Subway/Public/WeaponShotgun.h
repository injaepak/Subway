// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "WeaponShotgun.generated.h"

UCLASS()
class SUBWAY_API AWeaponShotgun :public AWeaponBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponShotgun();

	virtual void Fire() override;
	virtual void Reload() override;

	UPROPERTY()
	bool needToReroad;

	class APickUpActor* pistolParentActor;
	class AShotGunActor* shotGunParentActor;
};
