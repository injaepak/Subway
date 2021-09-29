// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponShotgun.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSPlayer.h" // �׽�Ʈ�� �÷��̾� ����
#include "Kismet/GameplayStatics.h" // ��ƼŬ�ý��� ���� ����
#include "TriggerBoxBase.h" // Ʈ���Źڽ����� �˻縦 ���� ����
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
			FCollisionObjectQueryParams QuaryParams;

			CollisionParams.AddIgnoredActor(this);
			// �÷��̾� �����̸� �־��ָ� �÷��̾ ������
			//CollisionParams.AddIgnoredActor(ShootingPlayer);

			// ź�� �Ҹ�
			CurrentMagazineAmmo--;
			// �������� �ʿ��� ���·� ��ȯ
			needToReroad = true;

			//bool bHit = GetWorld()->LineTraceSingleByChannel(HitResults, Start, End, ECollisionChannel::ECC_GameTraceChannel12, CollisionParams, CollisionResponse);
			//bool bHit = GetWorld()->LineTraceSingleByChannel(HitResults, Start, End, ECollisionChannel::ECC_Pawn, CollisionParams, CollisionResponse);
			
			// LineTrace�� 5000.f �Ÿ����� �߻�
			bool bHit = GetWorld()->LineTraceSingleByObjectType
			(HitResults, Start, End, QuaryParams, CollisionParams);

			// Hit���� �ʾҴ��� ���� ź�� �� ����Ʈ�� ���
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Extra Ammo: %d"), CurrentMagazineAmmo));

			if (bHit)
			{
				// ����Ʈ���̽� �߻� �� ����׶��� ����
				DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.f, 0.f, 5.f);

				// ����Ʈ���̽� �߻� �� ����׶��� ���� �� Shoot Effect ��ƼŬȿ�� ���
				FTransform startTrans2;
				startTrans2.SetLocation(Start + Rot * 1.0f);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletShootEffect, startTrans2);

				// �浹�� �߻��ߴٸ�
				if (HitResults.GetComponent()->GetAttachmentRootActor() != NULL) // ������Ʈ��(Brush Ÿ��)�� �� ũ���� ���� �ʰ� �Ѵ�
				{
					//Ÿ���� ����� �̸��� ���
					//debugMessage
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, HitResults.GetActor()->GetName());
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, HitResults.GetComponent()->GetName());

					// ����Ʈ���̽� �ε����� �� �ε��� ������ ��ƼŬȿ�� ���
					FTransform hitTrans;
					hitTrans.SetLocation(HitResults.ImpactPoint);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletHitEffect, hitTrans);

					// ----------------------------------------------------------------------

					// ���� Ʈ���Źڽ��� �ƴٸ� �� ���� ������ true�� ����
					auto triggerBox = Cast<ATriggerBoxBase>(HitResults.GetActor());

					if (triggerBox)
					{

						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("TRIGGER IN!!")));
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%d"), triggerBox->openTheDoor));

						triggerBox->openTheDoor = true;

						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%d"), triggerBox->openTheDoor));

					}
				}

				//������, ������, ����, persistentLine ����, LifeTime, Thickness
				//DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.f, 0.f, 1.f);
				
				// ��ü ����� �����Ƿ� ����׽��Ǿ Hit�� Actor�� ��ġ��, ��ü�� ũ�⸸ŭ, 2�� ���� �����ش�.
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


				// EnemyA�� ������ ó��
				// EnemyB�� ������ ó��
				auto enemyA = Cast<AEnemyA>(HitResults.GetActor());
				auto enemyB = Cast<AEnemyB>(HitResults.GetActor());
				if (enemyA)
				{
					if (HitResults.GetComponent()->GetName().Contains(TEXT("HeadCollision")))
					{
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("HEAD!!")));
						enemyA->enemyAFSM->OnDamageProcess(6.f, Rot, true);
					}
					else if (HitResults.GetComponent()->GetName().Contains(TEXT("BoxCollision")))
					{
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("BODY!!")));
						enemyA->enemyAFSM->OnDamageProcess(3.f, Rot, false);
					}
				}
				else if (enemyB)
				{
					if (HitResults.GetComponent()->GetName().Contains(TEXT("HeadCollision")))
					{
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("HEAD!!")));
						enemyB->enemyBFSM->OnDamageProcess(6.f, Rot, true);
					}
					else if (HitResults.GetComponent()->GetName().Contains(TEXT("BoxCollision")))
					{
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("BODY!!")));
						enemyB->enemyBFSM->OnDamageProcess(3.f, Rot, false);
					}
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

