// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrabActorComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUBWAY_API UGrabActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);

private:
	void ShowGrabLine();
	void HideGrabLine();
	void DrawGrabLine();
	void GrabAction();
	void ReleaseAction();
	void Test1();
	void Test2();
	void Fire();
	void Reload();

	bool bIsShowing = false;
	class APickUpActor* pickObject;
	class AShotGunActor* shotgunobject;
	class AVR_Player* player;
	class AWeaponPistol* pistol;
	class AWeaponShotgun* shotgun;
	
	FHitResult grabObject;

public:

	UPROPERTY(EditAnywhere, Category = GravSettings)
	float grabDistance = 1000.f;

	UPROPERTY(EditAnywhere, Category = GravSettings)
	class UHapticFeedbackEffect_Base* shotHaptic;

	bool bIsPistol = false;
	bool bIsShotgun = false;
};
