// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPistol.h"
#include "Components/SkeletalMeshComponent.h"
//#include ""
#include "DrawDebugHelpers.h"
#include "FPSPlayer.h" // 테스트용 플레이어 참조
#include "Kismet/GameplayStatics.h" // 파티클시스템 위해 참조
#include "TriggerBoxBase.h" // 트리거박스인지 검사를 위해 참조
#include "PickUpActor.h" // 총이 부모 클래스를 치지 않도록 참조
#include "ShotGunActor.h" // 총이 부모 클래스를 치지 않도록 참조
#include "EnemyA.h"
#include "EnemyA_FSM.h"
#include "EnemyB.h"
#include "EnemyB_FSM.h"
#include "Boss.h" // 보스를 참조
#include "Boss_FSM.h" // 보스FSM의 OnDamageProcess 접근할 수 있도록 참조
#include "GunTargetActor.h"
#include "DoorOpenActor.h"
#include "PistolAnimInstance.h" // 애님인스턴스 참조

// Sets default values
AWeaponPistol::AWeaponPistol()
{	
	WeaponName = "Pistol";
	BaseDamage = 100;
	WeaponMaxAmmo = 98;
	MagazineMaxAmmo = 15;

	CurrentTotalAmmo = WeaponMaxAmmo;
	CurrentMagazineAmmo = MagazineMaxAmmo;

	//pistolParentActor = Cast<APickUpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), APickUpActor::StaticClass()));
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickUpActor::StaticClass(), actors);
	for (auto tgt : actors)
	{
		pistolParentActor = Cast<APickUpActor>(tgt);
		break;
	}
	
	shotGunParentActor = Cast<AShotGunActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AShotGunActor::StaticClass()));
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Extra Ammo: %d"), currentAmmo));

}

void AWeaponPistol::Fire()
{
	// mesh 가필요
	// AnimInstance
	/*auto animInstance = Cast<UPistolAnimInstance>(WeaponMesh->GetAnimInstance());
	animInstance->Montage_Play(anim);*/
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
			CollisionParams.AddIgnoredActor(pistolParentActor);
			CollisionParams.AddIgnoredActor(shotGunParentActor);
			// 플레이어 변수이름 넣어주면 플레이어도 무시함
			//CollisionParams.AddIgnoredActor(ShootingPlayer);

			// ImpactPoint를 이용할 것이므로, 맞은 대상의 위치 Tramsform을 알아낼 변수 선언
			FTransform hitTrans;

			CurrentMagazineAmmo--;

			//bool bHit = GetWorld()->LineTraceSingleByChannel(HitResults, Start, End, ECollisionChannel::ECC_GameTraceChannel12, CollisionParams, CollisionResponse);
			bool bHit = GetWorld()->LineTraceSingleByChannel(HitResults, Start, End, ECollisionChannel::ECC_Pawn, CollisionParams, CollisionResponse);

			// Hit하지 않았더라도 남은 탄약 수 뷰포트상에 출력
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Extra Ammo: %d"), CurrentMagazineAmmo));

			if (bHit)
			{
				// 라인트레이스 발사 시 디버그라인 생성
				DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.f, 0.f, 1.f);

				// 라인트레이스 발사 시 디버그라인 생성 후 Shoot Effect 파티클효과 재생
				FTransform startTrans;
				startTrans.SetLocation(Start + Rot * 1.0f);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletShootEffect, startTrans);

				// 충돌이 발생했다면
				if (HitResults.GetComponent()->GetAttachmentRootActor() != NULL) // 지오메트리(Brush 타입)일 때 크래시 나지 않게 한다
				{
					//타격한 대상의 이름을 출력
					//debugMessage
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, HitResults.GetActor()->GetName());
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, HitResults.GetComponent()->GetName());

					// 라인트레이스 부딪혔을 때 부딪힌 지점에 파티클효과 재생

					auto worldHitPoint = Cast<UStaticMesh>(HitResults.GetComponent());
					hitTrans.SetLocation(HitResults.ImpactPoint);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletAnotherHitEffect, hitTrans);
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, HitResults.GetComponent()->GetName());

					// ----------------------------------------------------------------------

					// 만약 트리거박스를 쳤다면 문 여는 변수를 true로 변경
					auto triggerBox = Cast<ATriggerBoxBase>(HitResults.GetActor());

					if (triggerBox)
					{

						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("TRIGGER IN!!")));
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%d"), triggerBox->openTheDoor));

						triggerBox->openTheDoor = true;

						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%d"), triggerBox->openTheDoor));

					}
					// 만약 GunTargetActor를 쳤다면
					auto GunTargetActor = Cast<AGunTargetActor>(HitResults.GetActor());
					if (GunTargetActor)
					{
						GunTargetActor->OnDamageProcess();
					}
					//만약 DoorOpenActor를 쳤다면
					auto DoorOpenActor = Cast<ADoorOpenActor>(HitResults.GetActor());
					if (DoorOpenActor)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%d"), triggerBox->openTheDoor));
						DoorOpenActor->OnDamageProcess();
					}
				}
				//debugMessage
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, HitResults.GetActor()->GetName());
				//DrawDebugLine(GetWorld(), Start, End, FColor(255, 0, 0), false, 1.f, 0.f, 10.f); // 지속시간 수정
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


				// EnemyA에 데미지 처리
				// EnemyB에 데미지 처리
				// Boss 에 데미지 처리
				// Enemy, Boss 를 쳤다면 bulletEnemyHitEffect 를 Spawn
				auto enemyA = Cast<AEnemyA>(HitResults.GetActor());
				auto enemyB = Cast<AEnemyB>(HitResults.GetActor());
				auto boss = Cast<ABoss>(HitResults.GetActor());
				if (enemyA)
				{
					if (HitResults.GetComponent()->GetName().Contains(TEXT("HeadCollision")))
					{
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("HEAD!!")));
						enemyA->enemyAFSM->OnDamageProcess(3.f, Rot, 50, true);
					}
					else if (HitResults.GetComponent()->GetName().Contains(TEXT("BoxCollision")))
					{
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("BODY!!")));
						enemyA->enemyAFSM->OnDamageProcess(1.f, Rot, 50, false);
					}
					// 맞은 대상이 EnemyA이므로 EnemyHit 이펙트를 Spawn
					hitTrans.SetLocation(HitResults.ImpactPoint);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEnemyHitEffect, hitTrans);
					//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Enemy를 타격!!!!")));



				}
				else if (enemyB)
				{
					if (HitResults.GetComponent()->GetName().Contains(TEXT("HeadCollision")))
					{
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("HEAD!!")));
						enemyB->enemyBFSM->OnDamageProcess(3.f, Rot, 50, true);
					}
					else if (HitResults.GetComponent()->GetName().Contains(TEXT("BoxCollision")))
					{
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("BODY!!")));
						enemyB->enemyBFSM->OnDamageProcess(1.f, Rot, 50, false);
					}
					// 맞은 대상이 EnemyB이므로 EnemyHit 이펙트를 Spawn
					hitTrans.SetLocation(HitResults.ImpactPoint);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEnemyHitEffect, hitTrans);
					//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Enemy를 타격!!!!")));



				}
				//--------------------------------
						// Boss 타격 시 데미지프로세스 실행하고, 이펙트 스폰
				else if (boss)	// ▶ 주석 시작
				{
					if (HitResults.GetComponent()->GetName().Contains(TEXT("HeadCollision"))) // ▶ 콜리젼 구분 있을 시 if, else if 주석 켜시면 됩니다
					//if (Hit.GetComponent()->GetName().Contains(TEXT("HeadCollision")))
					{
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("HEAD!!")));
						boss->BossFSM->OnDamageProcess(6.f, Rot, 50, true);
					}
					else if (HitResults.GetComponent()->GetName().Contains(TEXT("BoxCollision")))
						//else if (Hit.GetComponent()->GetName().Contains(TEXT("BoxCollision")))
					{
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("BODY!!")));
						boss->BossFSM->OnDamageProcess(3.f, Rot, 50, false);
					}

					/*else if (Hit.GetComponent()->GetName().Contains(TEXT("RtArmCollision")))
					{
						boss->BossFSM->OnDamageProcess(3.f, Rot, false);
					}
					else if (Hit.GetComponent()->GetName().Contains(TEXT("LtArmCollision")))
					{
						boss->BossFSM->OnDamageProcess(3.f, Rot, false);
					}*/

					// 맞은 대상이 Boss이므로 EnemyHit 이펙트를 Spawn
					//hitTrans.SetLocation(Hit.ImpactPoint);
					//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEnemyHitEffect, hitTrans);
					//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Boss를 타격!!!!")));


				}	// ▶ 주석 끝
				//--------------------------------
			}
		}
		//else
		//{
		////GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString(TEXT("Need To Reload!!!")));
		//}
}

void AWeaponPistol::Reload()
{
		CurrentMagazineAmmo = MagazineMaxAmmo;
}

