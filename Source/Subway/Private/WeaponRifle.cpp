// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponRifle.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSPlayer.h" // VRPlayer ��Ŭ������ֽø� �ɰͰ����ϴ�!
#include "EnemyA.h"
#include "EnemyA_FSM.h"
#include "EnemyB.h"
#include "EnemyB_FSM.h"

AWeaponRifle::AWeaponRifle() // �����ڿ��� �Ӽ� ����(�Ӽ� ������ Base���Ͽ� ����)
{
	WeaponName = "Rifle";
	BaseDamage = 500;
	WeaponMaxAmmo = 100; // ����
	MagazineMaxAmmo = 50;

	CurrentTotalAmmo = WeaponMaxAmmo; // ����
	CurrentMagazineAmmo = MagazineMaxAmmo;
}

void AWeaponRifle::Fire()
{
		// (���� ������)
		for (int i = 0; i < 6; i++) // for���� ����Ǹ�
			/*if (count > 5)
			{
			GetWorldTimerManager().ClearTimer(WaitHandle);
		}*/
		{
			FTimerHandle WaitHandle; // 0.2�� �����̸� �� �� To do list �����Ѵ� for���� ����Ƚ����ŭ �ݺ��Ѵ�
			WaitTime = 0.2f; // ���� �ӵ�
			GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
				{
					if (CurrentMagazineAmmo > 0) // ���� �Ѿ��� 0������ ���ٸ�
					{
						UE_LOG(LogTemp, Warning, TEXT("SHOOTING RIFLE 6SHOT")); // �������� 6�� źȯ�� �߻��Ѵ�


						FVector Start = WeaponMesh->GetBoneLocation(FName("b_gun_muzzleflash")); // �������� ���� ����&����
						FVector Rot = WeaponMesh->GetBoneQuaternion(FName("b_gun_muzzleflash")).Vector(); // ����ȸ���� ���� ����&����
						FVector End = Start + Rot * 5000.0f; // �𸮾��� cm����. ��Ÿ� 50m

						//TArray<FHitResult> HitResults;
						FHitResult HitResults; // ����Ʈ���̽��� �ε��� ����� ������ ���� ������ ���� 
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

				//	count++
				}), WaitTime,true); //�ݺ��� ���⼭ �߰� ������ ������ ��������
		} // for �� ��
	
	// ������ ������
	
	// ���簡 �����ϸ� �ʹ� ���ϱ� �����̸� �� ���� ( �̰� ������ bool ���� �Ἥ isCooling���¶�� ���;� �ϴ� ������. ���� ���� ����)
	//FTimerHandle WaitHandle; // 1�� �����̸� �� �� To do list �����Ѵ� for���� ����Ƚ����ŭ �ݺ��Ѵ�
	//WaitTime2 = 10.0f; // ���� �ӵ�
	//GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString(TEXT("2 Sec Wait...")));
	//	}), WaitTime2, false); //�ݺ��� ���⼭ �߰� ������ ������ ��������
}

void AWeaponRifle::Reload()
{
		CurrentMagazineAmmo = MagazineMaxAmmo;
}
