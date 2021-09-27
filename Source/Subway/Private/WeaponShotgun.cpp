// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponShotgun.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSPlayer.h" // �׽�Ʈ�� �÷��̾� ����
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
	// �������� �ʿ��� ���¶��
	if(needToReroad == true)
	{GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("put the bullet")));}
	// �ƴ϶�� 
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
			// �÷��̾� �����̸� �־��ָ� �÷��̾ ������
			//CollisionParams.AddIgnoredActor(ShootingPlayer);

			// ź�� �Ҹ�
			CurrentMagazineAmmo--;
			// �������� �ʿ��� ���·� ��ȯ
			needToReroad = true;

			//bool bHit = GetWorld()->LineTraceSingleByChannel(HitResults, Start, End, ECollisionChannel::ECC_GameTraceChannel12, CollisionParams, CollisionResponse);
			//bool bHit = GetWorld()->LineTraceSingleByChannel(HitResults, Start, End, ECollisionChannel::ECC_Pawn, CollisionParams, CollisionResponse);
			
			// Sphere ������ SweepTrace�� 5000.f �Ÿ����� �߻�
			bool bHit = GetWorld()->SweepSingleByChannel(HitResults, Start, End, FQuat::Identity, ECollisionChannel::ECC_WorldStatic, FCollisionShape::MakeSphere(10.0f), CollisionParams, CollisionResponse);


			// Hit���� �ʾҴ��� ���� ź�� �� ����Ʈ�� ���
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Extra Ammo: %d"), CurrentMagazineAmmo));

			if (bHit)
			{
				//������, ������, ����, persistentLine ����, LifeTime, Thickness
				//DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.f, 0.f, 1.f);
				
				// ��ü ����� �����Ƿ� ����׽��Ǿ Hit�� Actor�� ��ġ��, ��ü�� ũ�⸸ŭ, 2�� ���� �����ش�.
				DrawDebugSphere(GetWorld(), HitResults.GetActor()->GetActorLocation(), 50, 30, FColor::Red, false, 2.0f, 10, 0);

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


				// EnemyA�� ������ ó��
				// EnemyB�� ������ ó��
				auto enemyA = Cast<AEnemyA>(HitResults.GetActor());
				auto enemyB = Cast<AEnemyB>(HitResults.GetActor());
				
				if (enemyA)
				{
					enemyA->enemyAFSM->OnDamageProcess();
				}
				else if (enemyB)
				{
					enemyB->enemyBFSM->OnDamageProcess();
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
		// ���� ���� źâ�� 7���� �۴ٸ�
	if (CurrentMagazineAmmo < 7)
	{
		// źȯ�� 1�� ����
		CurrentMagazineAmmo++;
		needToReroad = false;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString(TEXT("No reload required!!!")));
	}
}

