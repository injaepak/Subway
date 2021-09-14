// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WeaponBase.h"
#include "FPSPlayer.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FPlayerInputDelegate, class UInputComponent*);
UCLASS()
class SUBWAY_API AFPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// StartingWeaponClass 프로퍼티에 AWeaponBase 타입인 애들만 할당(TSubclassOf)해줘
	UPROPERTY(EditAnywhere, Category = "Test")
	TSubclassOf<class AWeaponBase> StartingWeaponClass; 

	class AWeaponBase* CurrentWeapon;
	int32 WeaponIndex;
	TArray <AWeaponBase*> WeaponArray;

	bool blsAiming;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, Category = FPSCamera)
	class UCameraComponent* fpsCamera;

	UPROPERTY(VisibleAnywhere, Category = BodyMesh)
	class USkeletalMeshComponent* bodyMesh;

	UPROPERTY(VisibleAnywhere, Category = PlayerMove)
	class UPlayerMove* playerMove;

	FPlayerInputDelegate OnInputDelegate;

	void OnFire();
	void Reload();
};
