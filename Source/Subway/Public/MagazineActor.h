// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagazineActor.generated.h"

UCLASS()
class SUBWAY_API AMagazineActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagazineActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnComponentBeginOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = PickUPSettings)
		class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere, Category = PickUPSettings)
		class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, Category = PickUPSettings)
	class AVR_Player* player;

	UPROPERTY(EditAnywhere, Category = PickUPSettings)
	class APickUpActor* pickupActor;

	UPROPERTY(EditAnywhere, Category = PickUPSettings)
	class AShotGunActor* shotgunActor;

	UPROPERTY(EditAnywhere, Category = PickUPSettings)
	class AWeaponPistol* pistol;

	UPROPERTY(EditAnywhere, Category = PickUPSettings)
	class AWeaponShotgun* shotgun;

	// 소켓에 넣을떄 Offset 값을 조정
	UPROPERTY(EditAnywhere, Category = PickUPSettings)
		FVector grabOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		class USoundBase* gunMagSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		class USoundBase* shotgunMagSound;
};
