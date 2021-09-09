// Fill out your copyright notice in the Description page of Project Settings.


#include "VR_Player.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AVR_Player::AVR_Player()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CpasuleCollider"));
    SetRootComponent(capsuleComp);
    capsuleComp->SetCapsuleHalfHeight(65.0f);
    capsuleComp->SetCapsuleRadius(40.0f);

    cameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Camera Root"));
    cameraRoot->SetupAttachment(RootComponent);
    cameraRoot->SetRelativeLocation(FVector(0, 0, 40.0f));

    playerCam = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
    playerCam->SetupAttachment(cameraRoot);

    leftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftMotionController"));
    leftController->SetupAttachment(cameraRoot);
    leftController->MotionSource = "Left";
    

    rightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightMotionController"));
    rightController->SetupAttachment(cameraRoot);
    rightController->MotionSource = "Right";

    bodyComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
    bodyComp->SetupAttachment(RootComponent);
    bodyComp->SetRelativeScale3D(FVector(0.8f, 0.8f, 1.2f));

    leftHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Left Hand"));
    leftHand->SetupAttachment(leftController);
    leftHand->SetRelativeRotation(FRotator(0, 0, -90.0f));
    leftHand->SetRelativeScale3D(FVector(1.0f, -1.0f, 1.0f));

    rightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Right Hand"));
    rightHand->SetupAttachment(rightController);
    rightHand->SetRelativeRotation(FRotator(0, 0, 90.0f));

    //플레이어 컨트롤러 빙의
    AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AVR_Player::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVR_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVR_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

