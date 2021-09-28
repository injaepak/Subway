// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPistol.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSPlayer.h" // �׽�Ʈ�� �÷��̾� ����
#include "EnemyA.h"
#include "EnemyA_FSM.h"
#include "EnemyB.h"
#include "EnemyB_FSM.h"

// Sets default values
AWeaponPistol::AWeaponPistol()
{
	WeaponName = "Pistol";
	BaseDamage = 100;
	WeaponMaxAmmo = 98;
	MagazineMaxAmmo = 15;

	CurrentTotalAmmo = WeaponMaxAmmo;
	CurrentMagazineAmmo = MagazineMaxAmmo;
}

void AWeaponPistol::Fire()
{
		if (CurrentMagazineAmmo > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("SHOOTING PISTOL"));
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

			CurrentMagazineAmmo--;

			//bool bHit = GetWorld()->LineTraceSingleByChannel(HitResults, Start, End, ECollisionChannel::ECC_GameTraceChannel12, CollisionParams, CollisionResponse);
			bool bHit = GetWorld()->LineTraceSingleByChannel(HitResults, Start, End, ECollisionChannel::ECC_Pawn, CollisionParams, CollisionResponse);

			// Hit���� �ʾҴ��� ���� ź�� �� ����Ʈ�� ���
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Extra Ammo: %d"), CurrentMagazineAmmo));

			if (bHit)
			{
				DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.f, 0.f, 1.f);
				if (HitResults.GetComponent()->GetAttachmentRootActor() != NULL) // ������Ʈ��(Brush Ÿ��)�� �� ũ���� ���� �ʰ� �Ѵ�
				{
					//Ÿ���� ����� �̸��� ���
					//debugMessage
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, HitResults.GetActor()->GetName());
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, HitResults.GetComponent()->GetName());
				}
				//debugMessage
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, HitResults.GetActor()->GetName());
				//DrawDebugLine(GetWorld(), Start, End, FColor(255, 0, 0), false, 1.f, 0.f, 10.f); // ���ӽð� ����
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("HIT!!")));
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
					if (HitResults.GetComponent()->GetName().Contains(TEXT("HeadCollision")))
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("HEAD!!")));
					}
					else if (HitResults.GetComponent()->GetName().Contains(TEXT("BoxCollision")))
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("BODY!!")));
					}
				}
				else if (enemyB)
				{
					if (HitResults.GetComponent()->GetName().Contains(TEXT("HeadCollision")))
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("HEAD!!")));
					}
					else if (HitResults.GetComponent()->GetName().Contains(TEXT("BoxCollision")))
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("BODY!!")));
					}
				}

				/*if (enemyA)
				{
					enemyA->enemyAFSM->OnDamageProcess(2.f, Rot);
				}
				else if (enemyB)
				{
					enemyB->enemyBFSM->OnDamageProcess(2.f, Rot);
				}*/
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString(TEXT("Need To Reload!!!")));
		}
}

void AWeaponPistol::Reload()
{
		CurrentMagazineAmmo = MagazineMaxAmmo;
}

