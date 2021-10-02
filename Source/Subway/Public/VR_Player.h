// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VR_Player.generated.h"

UCLASS()
class SUBWAY_API AVR_Player : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVR_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
public:

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class UCapsuleComponent* capsuleComp;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class UStaticMeshComponent* bodyComp;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class USceneComponent* weaponsMain;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class UStaticMeshComponent* magComp;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class UStaticMeshComponent* shotgunComp;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class UStaticMeshComponent* gunComp;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class USceneComponent* cameraRoot;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class UCameraComponent* playerCam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
		class UMotionControllerComponent* leftController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
		class UMotionControllerComponent* rightController;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class USkeletalMeshComponent* leftHand;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class USkeletalMeshComponent* rightHand;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class UMoveActorComponent* moveComp;

	UPROPERTY(VisibleAnywhere, Category = PlayerSettings)
		class UHandActorComponent* handComp;

	UPROPERTY(VisibleAnywhere, Category = PlayerSettings)
		class UGrabActorComponent* grabComp;

	UPROPERTY(EditAnywhere, Category = PickUPSettings)
		class UChildActorComponent* gun;

	UPROPERTY(EditAnywhere, Category = PickUPSettings)
		class UChildActorComponent* shotgun;

	UPROPERTY(EditAnywhere, Category = PickUPSettings)
		class UChildActorComponent* mag;

private:
	void Fire1();
	void Fire2();
	void HorizontalMove(float value);
	void VerticalMove(float value);
	void ResetHMD();
	void cameraFade();

	FRotator hmdRotate;
	FVector hmdLocation;

	FRotator WeaponsRotate;
	FVector WeaponsLocation;

	float weaponsRotateYaw;

	bool bCamera = false;
};
