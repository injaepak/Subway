// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponShotgun.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSPlayer.h" // 테스트용 플레이어 참조
#include "VR_Player.h" // VR플레이어 참조
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
#include "ShotGunAnimInstance.h" // 애님인스턴스 참조


// Sets default values
AWeaponShotgun::AWeaponShotgun()
{
	WeaponName = "ShotGun";
	BaseDamage = 200;
	WeaponMaxAmmo = 98;
	MagazineMaxAmmo = 7;

	CurrentTotalAmmo = WeaponMaxAmmo;
	
	CurrentMagazineAmmo = MagazineMaxAmmo; 
	pistolParentActor = Cast<APickUpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), APickUpActor::StaticClass()));
	shotGunParentActor = Cast<AShotGunActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AShotGunActor::StaticClass()));
	vrPlayer = Cast<AVR_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), AVR_Player::StaticClass()));
	fpsPlayer = Cast<AFPSPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AFPSPlayer::StaticClass()));
	//needToReroad = false;

	static ConstructorHelpers::FObjectFinder<UAnimSequence>fireAnimation(TEXT("AnimSequence'Game/FPS_Weapon_Bundle/Weapons/Meshes/M1_shotgun/SK_M1_Shotgun_X_Skeleton_Sequence.uasset'"));
	FireAnimation = fireAnimation.Object;
	
	bool bLoop = false;
	
	//PlayAnimation(FireAnimation, bLoop);
}

void AWeaponShotgun::Fire()
{
	// 재장전이 필요한 상태라면
	//if(needToReroad == true)
	//{GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("put the bullet")));}
	// 아니라면 
	/*else
	{*/
		if (CurrentMagazineAmmo > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("SHOOTING SHOTGUN"));
			FVector Start = WeaponMesh->GetBoneLocation(FName("b_shotgun_muzzleflash"));
			FVector Rot = WeaponMesh->GetForwardVector();
			FVector End = Start + Rot * 5000.f;

			TArray<FHitResult> HitResults;			
			FCollisionShape CollisionShape;

			CollisionShape.ShapeType = ECollisionShape::Sphere;
			CollisionShape.SetSphere(100.f);
			//*FHitResult HitResults;
			
			FCollisionQueryParams CollisionParams;
			FCollisionResponseParams CollisionResponse;
			FCollisionObjectQueryParams QuaryParams;

			CollisionParams.AddIgnoredActor(this); // 자기 자신 무시
			CollisionParams.AddIgnoredActor(pistolParentActor); // 피스톨의 픽업액터 무시
			CollisionParams.AddIgnoredActor(shotGunParentActor); // 샷건의 샷건액터 무시
			// 플레이어 변수이름 넣어주면 플레이어도 무시함
			CollisionParams.AddIgnoredActor(vrPlayer);
			CollisionParams.AddIgnoredActor(fpsPlayer);
			
			// ImpactPoint를 이용할 것이므로, 맞은 대상의 위치 Tramsform을 알아낼 변수 선언
			FTransform hitTrans;
			
			// 탄약 소모
			CurrentMagazineAmmo--;
			//Hit하지 않았더라도 남은 탄약 수 뷰포트상에 출력
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Extra Ammo: %d"), CurrentMagazineAmmo));
			// 재장전이 필요한 상태로 전환
			//needToReroad = true;

			//bool bHit = GetWorld()->LineTraceSingleByChannel(HitResults, Start, End, ECollisionChannel::ECC_GameTraceChannel12, CollisionParams, CollisionResponse);
			//bool bHit = GetWorld()->LineTraceSingleByChannel(HitResults, Start, End, ECollisionChannel::ECC_Pawn, CollisionParams, CollisionResponse);
			
			// LineTrace를 5000.f 거리까지 발사
			//*bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResults, Start, End, QuaryParams, CollisionParams);
			bool bHit = GetWorld()->SweepMultiByChannel(HitResults, Start, End, FQuat::FQuat(), ECC_WorldStatic, CollisionShape, CollisionParams);

			//------
			for (int32 i = 0; i < HitResults.Num(); i++)
			{
				FHitResult Hit = HitResults[i];
				
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, CollisionShape.GetSphereRadius(), 10.0f, FColor::Magenta, false, 10.0f, 1, 1.f);
				//DrawDebugSphere(GetWorld(), Start, CollisionShape.GetSphereRadius(), 10.0f, FColor::Magenta, true, 10.0f, 1, 1.f);
			//------
			

				if (bHit)
				{

					// 라인트레이스 발사 시 디버그라인 생성
					//*DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.f, 0.f, 5.f);

					// 총구에서 Shoot Effect 파티클효과 재생
					FTransform startTrans;
					startTrans.SetLocation(Start + Rot * 1.0f);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletShootEffect, startTrans);
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("총구 이펙트 스폰 성공")));

					// 충돌이 발생했다면
					if (Hit.GetComponent()->GetAttachmentRootActor() != NULL) // 지오메트리(Brush 타입)일 때 크래시 나지 않게 한다
					{
						//타격한 대상(액터)의 이름을 출력
						//debugMessage
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, Hit.GetActor()->GetName());
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, Hit.GetComponent()->GetName());

						// 사용안함. 부딪혔을 때 부딪힌 지점에 이펙트 스폰. 기본값 : bulletAnotherHitEffect.
						/*auto worldHitPoint = Cast<UStaticMesh>(Hit.GetComponent());
						hitTrans.SetLocation(Hit.ImpactPoint);
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletAnotherHitEffect, hitTrans);*/
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, HitResults.GetComponent()->GetName());

						// ----------------------------------------------------------------------

						// 만약 트리거박스를 쳤다면 문 여는 변수를 true로 변경
						auto triggerBox = Cast<ATriggerBoxBase>(Hit.GetActor());

						if (triggerBox)
						{

							//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("TRIGGER IN!!")));
							//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%d"), triggerBox->openTheDoor));

							triggerBox->openTheDoor = true;

							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%d"), triggerBox->openTheDoor));

						}
						// 만약 GunTargetActor를 쳤다면
						auto GunTargetActor = Cast<AGunTargetActor>(Hit.GetActor());
						if (GunTargetActor)
						{
							GunTargetActor->OnDamageProcess();
						}
						//만약 DoorOpenActor를 쳤다면
						auto DoorOpenActor = Cast<ADoorOpenActor>(Hit.GetActor());
						if (DoorOpenActor)
						{
							DoorOpenActor->OnDamageProcess();
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
					// Boss 에 데미지 처리
					// Enemy, Boss 를 쳤다면 bulletEnemyHitEffect 를 Spawn
					auto enemyA = Cast<AEnemyA>(Hit.GetActor());
					auto enemyB = Cast<AEnemyB>(Hit.GetActor());
					auto boss = Cast<ABoss>(Hit.GetActor());
					if (enemyA)
					{
						if (Hit.GetComponent()->GetName().Contains(TEXT("HeadCollision")))
						{
							//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("HEAD!!")));
							enemyA->enemyAFSM->OnDamageProcess(6.f, Rot, 100, true);
						}
						else if (Hit.GetComponent()->GetName().Contains(TEXT("BoxCollision")))
						{
							//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("BODY!!")));
							enemyA->enemyAFSM->OnDamageProcess(3.f, Rot, 100, false);
						}
						// 맞은 대상이 EnemyA이므로 EnemyHit 이펙트를 Spawn
						hitTrans.SetLocation(Hit.ImpactPoint);
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEnemyHitEffect, hitTrans);
						GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Magenta, FString::Printf(TEXT("에너미A 타격 이펙트 스폰 성공!")));

						//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Enemy를 타격!!!!")));

					}
					else if (enemyB)
					{
						if (Hit.GetComponent()->GetName().Contains(TEXT("HeadCollision")))
						{
							//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("HEAD!!")));
							enemyB->enemyBFSM->OnDamageProcess(6.f, Rot, 100, true);
						}
						else if (Hit.GetComponent()->GetName().Contains(TEXT("BoxCollision")))
						{
							//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("BODY!!")));
							enemyB->enemyBFSM->OnDamageProcess(3.f, Rot, 100, false);
						}
						// 맞은 대상이 EnemyB이므로 EnemyHit 이펙트를 Spawn
						hitTrans.SetLocation(Hit.ImpactPoint);
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEnemyHitEffect, hitTrans);
						GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Magenta, FString::Printf(TEXT("에너미B 타격 이펙트 스폰 성공!")));
						
						//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Enemy를 타격!!!!")));


					}
					//--------------------------------
					// Boss 타격 시 데미지프로세스 실행하고, 이펙트 스폰
					else if (boss)	// ▶ 주석 시작
					{
						if(Hit.GetComponent()->GetName().Contains(TEXT("HeadCollision"))) // ▶ 콜리젼 구분 있을 시 if, else if 주석 켜시면 됩니다
						//if (Hit.GetComponent()->GetName().Contains(TEXT("HeadCollision")))
						{
							//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("HEAD!!")));
							boss->BossFSM->OnDamageProcess(6.f, Rot, 100, true);
						}
						else if (Hit.GetComponent()->GetName().Contains(TEXT("BoxCollision"))) 
						//else if (Hit.GetComponent()->GetName().Contains(TEXT("BoxCollision")))
						{
							//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("BODY!!")));
							boss->BossFSM->OnDamageProcess(3.f, Rot, 100, false);
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
		}
		else
		{
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString(TEXT("Need To Reload!!!")));
		}
	//}
}

void AWeaponShotgun::Reload()
{
		// 만약 현재 탄창이 7보다 작다면
	if (CurrentMagazineAmmo < 7)
	{
		// 탄환을 1개 장전
		CurrentMagazineAmmo++;
		//needToReroad = false;
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString(TEXT("No reload required!!!")));
	}
}

