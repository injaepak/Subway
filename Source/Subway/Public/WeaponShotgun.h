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

	UFUNCTION(BlueprintCallable) // 블루프린트에서 불러올 수 있도록 UFUNCTION 선언
	virtual void Fire() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void Reload() override;

	UPROPERTY(EditAnywhere, Category = Setting) // 애님 몽타주 가져올 애님몽타주 타입의 anim 변수 선언
		class UAnimMontage* anim;

	UPROPERTY(BlueprintReadWrite)
		bool isFire = false;

	UPROPERTY(BlueprintReadWrite)
	FHitResult Hit;

	UPROPERTY()
	bool needToReroad;

	class APickUpActor* pistolParentActor;
	class AShotGunActor* shotGunParentActor;
	class AVR_Player* vrPlayer;
	class AFPSPlayer* fpsPlayer;
};
