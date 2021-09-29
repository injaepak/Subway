// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponShotgun.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSPlayer.h" // 테스트용 플레이어 참조
#include "Kismet/GameplayStatics.h" // 파티클시스템 위해 참조
#include "TriggerBoxBase.h" // 트리거박스인지 검사를 위해 참조
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
			FCollisionObjectQueryParams QuaryParams;

			CollisionParams.AddIgnoredActor(this);
			// 플레이어 변수이름 넣어주면 플레이어도 무시함
			//CollisionParams.AddIgnoredActor(ShootingPlayer);

			// 탄약 소모
			CurrentMagazineAmmo--;
			// 재장전이 필요한 상태로 전환
			needToReroad = true;

			//bool bHit = GetWorld()->LineTraceSingleByChannel(HitResults, Start, End, ECollisionChannel::ECC_GameTraceChannel12, CollisionParams, CollisionResponse);
			//bool bHit = GetWorld()->LineTraceSingleByChannel(HitResults, Start, End, ECollisionChannel::ECC_Pawn, CollisionParams, CollisionResponse);
			
			// LineTrace를 5000.f 거리까지 발사
			bool bHit = GetWorld()->LineTraceSingleByObjectType
			(HitResults, Start, End, QuaryParams, CollisionParams);

			// Hit하지 않았더라도 남은 탄약 수 뷰포트상에 출력
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Extra Ammo: %d"), CurrentMagazineAmmo));

			if (bHit)
			{
				// 라인트레이스 발사 시 디버그라인 생성
				DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.f, 0.f, 5.f);

				// 라인트레이스 발사 시 디버그라인 생성 후 Shoot Effect 파티클효과 재생
				FTransform startTrans2;
				startTrans2.SetLocation(Start + Rot * 1.0f);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletShootEffect, startTrans2);

				// 충돌이 발생했다면
				if (HitResults.GetComponent()->GetAttachmentRootActor() != NULL) // 지오메트리(Brush 타입)일 때 크래시 나지 않게 한다
				{
					//타격한 대상의 이름을 출력
					//debugMessage
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, HitResults.GetActor()->GetName());
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, HitResults.GetComponent()->GetName());

					// 라인트레이스 부딪혔을 때 부딪힌 지점에 파티클효과 재생
					FTransform hitTrans;
					hitTrans.SetLocation(HitResults.ImpactPoint);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletHitEffect, hitTrans);

					// ----------------------------------------------------------------------

					// 만약 트리거박스를 쳤다면 문 여는 변수를 true로 변경
					auto triggerBox = Cast<ATriggerBoxBase>(HitResults.GetActor());

					if (triggerBox)
					{

						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("TRIGGER IN!!")));
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%d"), triggerBox->openTheDoor));

						triggerBox->openTheDoor = true;

						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%d"), triggerBox->openTheDoor));

					}
				}

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

