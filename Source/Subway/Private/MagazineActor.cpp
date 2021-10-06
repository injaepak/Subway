// Fill out your copyright notice in the Description page of Project Settings.


#include "MagazineActor.h"
#include "VR_Player.h"
#include "Subway.h"
#include "PickUpActor.h"
#include "ShotGunActor.h"
#include "WeaponPistol.h"
#include "WeaponShotgun.h"
#include "GrabActorComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ChildActorComponent.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AMagazineActor::AMagazineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(boxComp);

	// 물리 관련 설정
	boxComp->SetSimulatePhysics(true);
	boxComp->SetEnableGravity(true);
	boxComp->SetGenerateOverlapEvents(true);
	boxComp->SetCollisionProfileName(TEXT("PickUP"));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(boxComp);

	/*mag = CreateDefaultSubobject<UChildActorComponent>(TEXT("Mag"));
	mag->SetupAttachment(RootComponent);*/
}

// Called when the game starts or when spawned
void AMagazineActor::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AMagazineActor::OnComponentBeginOverlap);
	player = Cast<AVR_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), AVR_Player::StaticClass()));
	pickupActor = Cast<APickUpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), APickUpActor::StaticClass()));
	pistol =Cast<AWeaponPistol>(UGameplayStatics::GetActorOfClass(GetWorld(), AWeaponPistol::StaticClass()));
	shotgun = Cast<AWeaponShotgun>(UGameplayStatics::GetActorOfClass(GetWorld(), AWeaponShotgun::StaticClass()));

}

// Called every frame
void AMagazineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMagazineActor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	pistol = Cast<AWeaponPistol>(OtherActor);
	shotgunActor = Cast<AShotGunActor>(OtherActor);
	
	if (pickupActor || pistol)
	{
		//PRINTLOG(TEXT("RRRRRRRRRRR"))
		
		pistol->Reload();
		player->grabComp->LeftReleaseAction();
		UGameplayStatics::PlaySound2D(GetWorld(), gunMagSound);
	}

	if (shotgunActor)
	{
		shotgun->Reload();
		player->grabComp->LeftReleaseAction();
		UGameplayStatics::PlaySound2D(GetWorld(), shotgunMagSound);
	}
}



