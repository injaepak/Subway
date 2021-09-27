// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponShotgun.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSPlayer.h" // 테스트용 플레이어 참조
#include "EnemyA.h"
#include "EnemyA_FSM.h"
#include "EnemyB.h"
#include "EnemyB_FSM.h"

// Sets default values
AWeaponShotgun::AWeaponShotgun()
{
	WeaponName = "ShotGun";
	BaseDamage = 200;
	WeaponMaxAmmo = 98;
	MagazineMaxAmmo = 7;

	CurrentTotalAmmo = WeaponMaxAmmo;
	CurrentMagazineAmmo = MagazineMaxAmmo;
	needToReroad = false;
}

void AWeaponShotgun::Fire()
{
	// 재장전이 필요한 상태라면
	if(needToReroad == true)
	{GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("put the bullet")));}
	// 아니라면 
	else
	{
		if (CurrentMagazineAmmo > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("SHOOTING SHOTGUN"));
			FVector Start = WeaponMesh->GetBoneLocation(FName("b_gun_muzzleflash"));
			FVector Rot = WeaponMesh->GetForwardVector();
			FVector End = Start + Rot * 5000.f;

			//TArray<FHitResult> HitResults;
			FHitResult HitResults;
			FCollisionQueryParams CollisionParams;
			FCollisionResponseParams CollisionResponse;
			CollisionParams.AddIgnoredActor(this);
			// 플레이어 변수이름 넣어주면 플레이어도 무시함
			//CollisionParams.AddIgnoredActor(ShootingPlayer);

			// 탄약 소모
			CurrentMagazineAmmo--;
			// 재장전이 필요한 상태로 전환
			needToReroad = true;

			//bool bHit = GetWorld()->LineTraceSingleByChannel(HitResults, Start, End, ECollisionChannel::ECC_GameTraceChannel12, CollisionParams, CollisionResponse);
			//bool bHit = GetWorld()->LineTraceSingleByChannel(HitResults, Start, End, ECollisionChannel::ECC_Pawn, CollisionParams, CollisionResponse);
			
			// Sphere 형태의 SweepTrace를 5000.f 거리까지 발사
			bool bHit = GetWorld()->SweepSingleByChannel(HitResults, Start, End, FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(50.0f), CollisionParams, CollisionResponse);


			// Hit하지 않았더라도 남은 탄약 수 뷰포트상에 출력
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Extra Ammo: %d"), CurrentMagazineAmmo));

			if (bHit)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Cause Damage: %d"), BaseDamage)); // 가한 데미지 출력
				//시작점, 종착점, 색상, persistentLine 유무, LifeTime, Thickness
				//DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.f, 0.f, 1.f);
				
				// 구체 모양을 쐈으므로 디버그스피어를 Hit한 Actor의 위치에, 구체의 크기만큼, 2초 동안 보여준다.
				//DrawDebugSphere(GetWorld(), HitResults.GetActor()->GetActorLocation(), 50, 30, FColor::Red, false, 2.0f, 10, 0);
				
				/*
				for (FHitResult& Result : HitResults)
				{
					if (AActor* HitActor = Result.GetActor())
					{
						UE_LOG(LogTemp, Warning, TEXT("Actor Name: %s"), *HitActor->GetName());
						GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("You are hitting: %s"), *HitActor->GetName()));
					}
				}
				*/


				// EnemyA에 데미지 처리
				// EnemyB에 데미지 처리
				auto enemyA = Cast<AEnemyA>(HitResults.GetActor());
				auto enemyB = Cast<AEnemyB>(HitResults.GetActor());
				
				if (enemyA)
				{
					enemyA->enemyAFSM->OnDamageProcess(4.f);
				}
				else if (enemyB)
				{
					enemyB->enemyBFSM->OnDamageProcess(4.f);
				}
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString(TEXT("Need To Reload!!!")));
		}
	}
}

void AWeaponShotgun::Reload()
{
		// 만약 현재 탄창이 7보다 작다면
	if (CurrentMagazineAmmo < 7)
	{
		// 탄환을 1개 장전
		CurrentMagazineAmmo++;
		needToReroad = false;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString(TEXT("No reload required!!!")));
	}
}

