// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "WeaponRifle.generated.h"

/**
 *
 */
UCLASS()
class SUBWAY_API AWeaponRifle : public AWeaponBase
{
	GENERATED_BODY()


public:
	AWeaponRifle();

	virtual void Fire() override;
	virtual void Reload() override;

	UPROPERTY(EditAnywhere, Category = "Test")
		float WaitTime;
};
