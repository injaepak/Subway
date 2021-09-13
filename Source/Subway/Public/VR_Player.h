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
		class USceneComponent* cameraRoot;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class UCameraComponent* playerCam;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class UMotionControllerComponent* leftController;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class UMotionControllerComponent* rightController;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class USkeletalMeshComponent* leftHand;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class USkeletalMeshComponent* rightHand;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		class UMoveActorComponent* moveComp;

private:
	void Fire1();
	void Fire2();
	void HorizontalMove(float value);
	void VerticalMove(float value);
};
