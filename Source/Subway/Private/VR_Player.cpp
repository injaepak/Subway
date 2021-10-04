// Fill out your copyright notice in the Description page of Project Settings.


#include "VR_Player.h"
#include "Subway.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MoveActorComponent.h"
#include "HeadMountedDisPlayFunctionLibrary.h"
#include "HandActorComponent.h"
#include "GrabActorComponent.h"
#include "PickUpActor.h"
#include "EnemyA.h"
#include "EnemyB.h"
#include <Kismet/GameplayStatics.h>


// Sets default values
AVR_Player::AVR_Player()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CpasuleCollider"));
    SetRootComponent(capsuleComp);
    capsuleComp->SetCapsuleHalfHeight(65.0f);
    capsuleComp->SetCapsuleRadius(40.0f);
    capsuleComp->SetCollisionProfileName(TEXT("VR_Player"));

    cameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Camera Root"));
    cameraRoot->SetupAttachment(RootComponent);
    cameraRoot->SetRelativeLocation(FVector(0, 0, 30.0f));

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

    weaponsMain = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponsMain"));
    weaponsMain->SetupAttachment(cameraRoot);

	magComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagMesh"));
    magComp->SetupAttachment(weaponsMain);

    shotgunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShotgunComp"));
    shotgunComp->SetupAttachment(weaponsMain);

	gunComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunComp"));
	gunComp->SetupAttachment(weaponsMain);

    leftHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Left Hand"));
    leftHand->SetupAttachment(leftController);
    leftHand->SetRelativeRotation(FRotator(0, 0, -90.0f));
    leftHand->SetRelativeScale3D(FVector(1.0f, -1.0f, 1.0f));
    
    rightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Right Hand"));
    rightHand->SetupAttachment(rightController);
    rightHand->SetRelativeRotation(FRotator(0, 0, 90.0f));

    moveComp = CreateDefaultSubobject<UMoveActorComponent>(TEXT("MoveComponent"));
    handComp = CreateDefaultSubobject<UHandActorComponent>(TEXT("HandComponent"));
    grabComp = CreateDefaultSubobject<UGrabActorComponent>(TEXT("GrabComponent"));

	gun = CreateDefaultSubobject<UChildActorComponent>(TEXT("Gun"));
	gun->SetupAttachment(weaponsMain);
   
	shotgun = CreateDefaultSubobject<UChildActorComponent>(TEXT("Shotgun"));
	shotgun->SetupAttachment(weaponsMain);

	mag = CreateDefaultSubobject<UChildActorComponent>(TEXT("Mag"));
	mag->SetupAttachment(weaponsMain);

    //플레이어 컨트롤러 빙의
    AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void AVR_Player::BeginPlay()
{
	Super::BeginPlay();
	
    // 헤드 마운트 디스플레이 장치의 초기 위치값을 설정하기
    UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(hmdRotate, hmdLocation);

    // HMD 장치의 기준점을 바닥으로 설정하기
    UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);

    // HMD 장치의 위치를 초기화하기
    UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();

   
}

// Called every frame
void AVR_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(WeaponsRotate, WeaponsLocation);
    weaponsRotateYaw = WeaponsRotate.Yaw;
    weaponsMain->SetRelativeRotation(FRotator(0.f, weaponsRotateYaw, 0.f));

	enemyA = Cast<AEnemyA>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyA::StaticClass()));
	enemyB = Cast<AEnemyB>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyB::StaticClass()));

   
   // EnemyA 플레이어 데미지 처리
    if(enemyA)
    {
        if (enemyA->bCanAttack == true)
	    {
            enemyAFirstCurrentTime += GetWorld()->DeltaTimeSeconds;
            
            if(bEnemyAtime == false)
            {
                if (enemyAFirstCurrentTime >= 1.5f)
                {
                    OnDamageProcess();
                    PRINTLOG(TEXT("First"));
                    bEnemyAtime = true;
                }
            }

            else if(bEnemyAtime == true)
            {
                enemyASecondCurrentTime += GetWorld()->DeltaTimeSeconds;
                    if (enemyASecondCurrentTime >= 3.8f)
                    {
                        OnDamageProcess();
				        PRINTLOG(TEXT("Second"));
                        enemyASecondCurrentTime = 0.f;
                    }
            }
        }
		else if (enemyA->bCanAttack == false && (!enemyB || enemyB->bCanAttack == false))
		{
			healCurrentTime += GetWorld()->DeltaTimeSeconds;
			if (healCurrentTime >= 5.f && playerHP <= 9)
			{
				cameraFadeHeal();
				PRINTLOG(TEXT("Heal"));
				playerHP++;
				healCurrentTime = 0;
			}
		}
    }

    // EnemyB 플레이어 데미지 처리
	if (enemyB)
	{
		if (enemyB->bCanAttack == true)
		{
            enemyBFirstCurrentTime += GetWorld()->DeltaTimeSeconds;

			if (bEnemyBtime == false)
			{
				if (enemyBFirstCurrentTime >= 1.f)
				{
					OnDamageProcess();
					PRINTLOG(TEXT("First"));
                    bEnemyBtime = true;
				}
			}

			else if (bEnemyBtime == true)
			{
                enemyBSecondCurrentTime += GetWorld()->DeltaTimeSeconds;
				if (enemyBSecondCurrentTime >= 4.2f)
				{
					OnDamageProcess();
					PRINTLOG(TEXT("Second"));
                    enemyBSecondCurrentTime = 0.f;
				}
			}
		}
		else if (enemyB->bCanAttack == false && (!enemyA || enemyA->bCanAttack == false))
		{
			healCurrentTime += GetWorld()->DeltaTimeSeconds;
			if (healCurrentTime >= 5.f && playerHP <= 9)
			{
				cameraFadeHeal();
				PRINTLOG(TEXT("Heal"));
				playerHP++;
				healCurrentTime = 0;
			}
		}
	}
    else if (!enemyA && !enemyB)
    {
        healCurrentTime += GetWorld()->DeltaTimeSeconds;
        if (healCurrentTime >= 5.f && playerHP <= 9)
        {
            cameraFadeHeal();
            PRINTLOG(TEXT("Heal"));
            playerHP++;
            healCurrentTime = 0;
        }
        
    }
    
}

// Called to bind functionality to input
void AVR_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    moveComp->SetupPlayerInputComponent(PlayerInputComponent);
    grabComp->SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("HMDReset", IE_Pressed, this, &AVR_Player::ResetHMD);

    // 액션 바인딩
    //PlayerInputComponent->BindAction("RightTrigger", IE_Pressed, this, &AVR_Player::Fire1);
    //PlayerInputComponent->BindAction("RightGrip", IE_Pressed, this, &AVR_Player::Fire2);

    // 액시스 바인딩
    //PlayerInputComponent->BindAxis("LeftThumbstick_X", this, &AVR_Player::HorizontalMove);
    //PlayerInputComponent->BindAxis("LeftThumbstick_Y", this, &AVR_Player::VerticalMove);

}

void AVR_Player::Fire1()
{
    PRINTLOG(TEXT("Fire1"));
}

void AVR_Player::Fire2()
{
    PRINTLOG(TEXT("Fire2"));
}

void AVR_Player::HorizontalMove(float value)
{
}

void AVR_Player::VerticalMove(float value)
{
}

void AVR_Player::ResetHMD()
{
    UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AVR_Player::cameraFade()
{
    bCamera = true;
    auto cameraCheck = FString::FromInt(bCamera);

    auto cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
    if (playerHP >= 9)
    {
        if (cameraManager)
        {
            cameraManager->StartCameraFade(0.f, 0.1f, 1.f, FLinearColor::Red, true, true);

            cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
            if (cameraManager)
            {
                cameraManager->StopCameraFade();
                cameraManager->StartCameraFade(0.1f, 0.f, 1.0f, FLinearColor::Red, false, false);
            }
        }
    }
	else if (playerHP <= 8 || playerHP >= 6)
    {
		if (cameraManager)
		{
			cameraManager->StartCameraFade(0.f, 0.5f, 2.0f, FLinearColor::Red, true, true);

			cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
			if (cameraManager)
			{
				cameraManager->StopCameraFade();
				cameraManager->StartCameraFade(0.5f, 0.f, 2.0f, FLinearColor::Red, false, false);
			}
		}
    }
	else if (playerHP <= 5 || playerHP >= 3)
    {
		if (cameraManager)
		{
			cameraManager->StartCameraFade(0.f, 0.7f, 4.0f, FLinearColor::Red, true, true);

			cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
			if (cameraManager)
			{
				cameraManager->StopCameraFade();
				cameraManager->StartCameraFade(0.7f, 0.f, 4.0f, FLinearColor::Red, false, false);
			}
		}
    }
	else if (playerHP <= 2 || playerHP >= 0)
    {
		if (cameraManager)
		{
			cameraManager->StartCameraFade(0.f, 0.9f, 5.0f, FLinearColor::Red, true, true);

			cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
			if (cameraManager)
			{
				cameraManager->StopCameraFade();
				cameraManager->StartCameraFade(0.9f, 0.f, 5.0f, FLinearColor::Red, false, false);

				PRINTLOG(TEXT("why"))
			}
		}
    }
}

void AVR_Player::cameraFadeHeal()
{
    if(playerHP <= 9)
    {
	    bCamera = true;
	    auto cameraCheck = FString::FromInt(bCamera);

	    auto cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		if (playerHP <= 2 || playerHP >= 0)
		{
			if (cameraManager)
			{
				cameraManager->StartCameraFade(0.f, 0.1f, 1.f, FLinearColor::Blue, true, true);

				cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
				if (cameraManager)
				{
					cameraManager->StopCameraFade();
					cameraManager->StartCameraFade(0.1f, 0.f, 1.f, FLinearColor::Blue, false, false);
				}
			}
		}
		else if (playerHP <= 5 || playerHP >= 3)
		{
			if (cameraManager)
			{
				cameraManager->StartCameraFade(0.f, 0.2f, 1.0f, FLinearColor::Blue, true, true);

				cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
				if (cameraManager)
				{
					cameraManager->StopCameraFade();
					cameraManager->StartCameraFade(0.2f, 0.f, 1.0f, FLinearColor::Blue, false, false);
				}
			}
		}
		else if (playerHP <= 8 || playerHP >= 6)
		{
			if (cameraManager)
			{
				cameraManager->StartCameraFade(0.f, 0.3f, 1.0f, FLinearColor::Blue, true, true);

				cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
				if (cameraManager)
				{
					cameraManager->StopCameraFade();
					cameraManager->StartCameraFade(0.3f, 0.f, 1.0f, FLinearColor::Blue, false, false);
				}
			}
		}
		else if (playerHP >= 9)
		{
			if (cameraManager)
			{
				cameraManager->StartCameraFade(0.f, 0.5f, 5.0f, FLinearColor::Blue, true, true);

				cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
				if (cameraManager)
				{
					cameraManager->StopCameraFade();
					cameraManager->StartCameraFade(0.5f, 0.f, 5.0f, FLinearColor::Blue, false, false);

					PRINTLOG(TEXT("why"))
				}
			}
		}
    }
}

void AVR_Player::OnDamageProcess()
{
    if (playerHP > 0)
    {
        cameraFade();
        
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("TEST HIT!!")));
        playerHP--;
        if (playerHP <=0)
        {
            this->Destroy();
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("DEADDD!!")));
        }
    }
}

void AVR_Player::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    enemyA = Cast<AEnemyA>(OtherActor);
    enemyB = Cast<AEnemyB>(OtherActor);

    if (enemyA->LtCollision || enemyA->RtCollision || enemyB->LtCollision || enemyB->RtCollision)
    {
        OnDamageProcess();
    }
}

