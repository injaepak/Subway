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

	UFUNCTION(BlueprintCallable)
	virtual void Fire() override;

	UFUNCTION(BlueprintCallable)
	virtual void Reload() override;

	//UPROPERTY(EditAnywhere, Category=Setting) // 애님 몽타주 가져올 애님몽타주 타입의 anim 변수 선언
	//class UAnimMontage* anim;

	class APickUpActor* pistolParentActor;
	class AShotGunActor* shotGunParentActor;
};
