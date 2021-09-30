// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSPlayer.h"
#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include "WeaponBase.h"
#include "PlayerMove.h"

// Sets default values
AFPSPlayer::AFPSPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	// 카메라컴포넌트 추가
	fpsCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	fpsCamera->SetupAttachment(GetCapsuleComponent());
	fpsCamera->SetRelativeLocation(FVector(-30, 0, 60));

	bodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	bodyMesh->SetupAttachment(fpsCamera);

	// playerMove
	playerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMove"));

	// bodyMesh 의 SkeletalMesh 데이터 할당하기
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/FirstPerson/Character/Mesh/SK_Mannequin_Arms.SK_Mannequin_Arms'"));

	// 로드가 성공하면
	if (tempMesh.Succeeded())
	{
		// 데이터 할당
		bodyMesh->SetSkeletalMesh(tempMesh.Object);
	}

	//AutoPossessPlayer = EAutoReceiveInput::Player0;

	//WeaponIndex = 0;
}

// Called when the game starts or when spawned
void AFPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	// 시작 시 소켓에 무기 스폰
	// 커런트웨폰 값을 월드의 스폰한 웨폰베이스타입의 스타팅웨폰클래스로 재정의함
	CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(StartingWeaponClass);
	if (CurrentWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("SPAWNED AND ATTEMPTED TO ATTACH WEAPON TO HAND"));
		CurrentWeapon->AttachToComponent(bodyMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RightGripPoint"));
		//WeaponArray.Add(CurrentWeapon);
	}
}

// Called every frame
void AFPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 총 발사
	OnInputDelegate.Broadcast(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AFPSPlayer::OnFire);
	// 재장전
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &AFPSPlayer::Reload);
}

void AFPSPlayer::OnFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Fire();
		TArray<int32> CurrentAmmo = CurrentWeapon->GetCurrentAmmo();
		OnAmmoChanged.Broadcast(CurrentAmmo[0], CurrentAmmo[1]);
	}
}

void AFPSPlayer::Reload()
{
	// 사용하는 무기의 탄창이 가득 찬 상태라면
	if (CurrentWeapon->CurrentMagazineAmmo == CurrentWeapon->MagazineMaxAmmo)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString(TEXT("No reload required!!!")));
	}
	else
	{
		CurrentWeapon->Reload();
	}
}

void AFPSPlayer::OnDamageProcess()
{
	if (PlayerHP > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("TEST HIT!!")));
		/*PlayerHP--;
		if (PlayerHP <=0)
		{
			this->Destroy();
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("DEADDD!!")));
		}*/
	}
}