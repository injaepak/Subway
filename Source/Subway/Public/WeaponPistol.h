// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "WeaponPistol.generated.h"

UCLASS()
class SUBWAY_API AWeaponPistol : public AWeaponBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponPistol();

	virtual void Fire() override;
	virtual void Reload() override;
};
