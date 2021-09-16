// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponShotgun.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSPlayer.h" // VRPlayer ��Ŭ������ֽø� �ɰͰ����ϴ�!
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

// inHand ���� true ��� �Լ��� �����ϰ�, �ƴ϶�� �������� �ʴ´�
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
			// �÷��̾� �����̸� �־��ָ� �÷��̾ ������
			//CollisionParams.AddIgnoredActor(ShootingPlayer);

			// ź�� �Ҹ�
			CurrentMagazineAmmo--;

			//bool bHit = GetWorld()->LineTraceSingleByChannel(HitResults, Start, End, ECollisionChannel::ECC_GameTraceChannel12, CollisionParams, CollisionResponse);
			bool bHit = GetWorld()->LineTraceSingleByChannel(HitResults, Start, End, ECollisionChannel::ECC_Pawn, CollisionParams, CollisionResponse);

			// Hit���� �ʾҴ��� ���� ź�� �� ����Ʈ�� ���
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Extra Ammo: %d"), CurrentMagazineAmmo));

			if (bHit)
			{
				//������, ������, ����, persistentLine ����, LifeTime, Thickness
				DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.f, 0.f, 1.f);
				//debugMessage
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, HitResults.GetActor()->GetName());
				//DrawDebugLine(GetWorld(), Start, End, FColor(255, 0, 0), false, 1.f, 0.f, 10.f); // ���ӽð� ����
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("HIT!!")));
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Cause Damage: %d"), BaseDamage)); // ���� ������ ���
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


				// Enemy�� ������ ó��
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

// inHand ���� true ��� �Լ��� �����ϰ�, �ƴ϶�� �������� �ʴ´�
void AWeaponShotgun::Reload()
{
		CurrentMagazineAmmo = MagazineMaxAmmo;
}

