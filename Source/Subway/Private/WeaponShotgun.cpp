// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponShotgun.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSPlayer.h" // VRPlayer 인클루드해주시면 될것같습니다!
#include "EnemyA.h"
#include "EnemyA_FSM.h"

// Sets default values
AWeaponShotgun::AWeaponShotgun()
{
	WeaponName = "ShotGun";
	BaseDamage = 200;
	WeaponMaxAmmo = 98;
	MagazineMaxAmmo = 7;

	CurrentTotalAmmo = WeaponMaxAmmo;
	CurrentMagazineAmmo = MagazineMaxAmmo;

}

// inHand 값이 true 라면 함수가 동작하고, 아니라면 동작하지 않는다
void AWeaponShotgun::Fire()
{

		if (CurrentMagazineAmmo > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("SHOOTING SHOTGUN"));
			FVector Start = WeaponMesh->GetBoneLocation(FName("b_gun_muzzleflash"));
			FVector Rot = WeaponMesh->GetBoneQuaternion(FName("b_gun_muzzleflash")).Vector();
			FVector End = Start + Rot * 5000.0f;

			//TArray<FHitResult> HitResults;
			FHitResult HitResults;
			FCollisionQueryParams CollisionParams;
			FCollisionResponseParams CollisionResponse;
			CollisionParams.AddIgnoredActor(this);
			// 플레이어 변수이름 넣어주면 플레이어도 무시함
			//CollisionParams.AddIgnoredActor(ShootingPlayer);

			// 탄약 소모
			CurrentMagazineAmmo--;

			//bool bHit = GetWorld()->LineTraceSingleByChannel(HitResults, Start, End, ECollisionChannel::ECC_GameTraceChannel12, CollisionParams, CollisionResponse);
			bool bHit = GetWorld()->LineTraceSingleByChannel(HitResults, Start, End, ECollisionChannel::ECC_Pawn, CollisionParams, CollisionResponse);

			// Hit하지 않았더라도 남은 탄약 수 뷰포트상에 출력
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Extra Ammo: %d"), CurrentMagazineAmmo));

			if (bHit)
			{
				//시작점, 종착점, 색상, persistentLine 유무, LifeTime, Thickness
				DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.f, 0.f, 1.f);
				//debugMessage
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, HitResults.GetActor()->GetName());
				//DrawDebugLine(GetWorld(), Start, End, FColor(255, 0, 0), false, 1.f, 0.f, 10.f); // 지속시간 수정
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("HIT!!")));
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Cause Damage: %d"), BaseDamage)); // 가한 데미지 출력
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


				// Enemy에 데미지 처리
				auto enemyA = Cast<AEnemyA>(HitResults.GetActor());
				if (enemyA)
				{
					enemyA->enemyAFSM->OnDamageProcess();
				}
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString(TEXT("Need To Reload!!!")));
		}
}

// inHand 값이 true 라면 함수가 동작하고, 아니라면 동작하지 않는다
void AWeaponShotgun::Reload()
{
		CurrentMagazineAmmo = MagazineMaxAmmo;
}

