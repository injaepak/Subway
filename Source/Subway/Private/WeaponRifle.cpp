// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponRifle.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSPlayer.h" // VRPlayer 인클루드해주시면 될것같습니다!
#include "EnemyA.h"
#include "EnemyA_FSM.h"
#include "EnemyB.h"
#include "EnemyB_FSM.h"

AWeaponRifle::AWeaponRifle() // 생성자에서 속성 설정(속성 선언은 Base파일에 있음)
{
	WeaponName = "Rifle";
	BaseDamage = 500;
	WeaponMaxAmmo = 100; // 비사용
	MagazineMaxAmmo = 50;

	CurrentTotalAmmo = WeaponMaxAmmo; // 비사용
	CurrentMagazineAmmo = MagazineMaxAmmo;
}

void AWeaponRifle::Fire()
{
		// (연사 수정중)
		for (int i = 0; i < 6; i++) // for문이 실행되면
			/*if (count > 5)
			{
			GetWorldTimerManager().ClearTimer(WaitHandle);
		}*/
		{
			FTimerHandle WaitHandle; // 0.2초 딜레이를 준 후 To do list 실행한다 for문의 설정횟수만큼 반복한다
			WaitTime = 0.2f; // 연사 속도
			GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
				{
					if (CurrentMagazineAmmo > 0) // 현재 총알이 0개보다 많다면
					{
						UE_LOG(LogTemp, Warning, TEXT("SHOOTING RIFLE 6SHOT")); // 라이플은 6개 탄환을 발사한다


						FVector Start = WeaponMesh->GetBoneLocation(FName("b_gun_muzzleflash")); // 시작지점 변수 선언&구현
						FVector Rot = WeaponMesh->GetBoneQuaternion(FName("b_gun_muzzleflash")).Vector(); // 시작회전값 변수 선언&구현
						FVector End = Start + Rot * 5000.0f; // 언리얼은 cm단위. 사거리 50m

						//TArray<FHitResult> HitResults;
						FHitResult HitResults; // 라인트레이스가 부딪힌 대상의 정보를 담을 변수를 설정 
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


							// EnemyA에 데미지 처리
							// EnemyB에 데미지 처리
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
				}), WaitTime,true); //반복도 여기서 추가 변수를 선언해 설정가능
		} // for 문 끝
	
	// 딜레이 수정중
	
	// 연사가 가능하면 너무 사기니까 딜레이를 좀 주자 ( 이거 원래는 bool 변수 써서 isCooling상태라면 나와야 하는 문구임. 추후 수정 예정)
	//FTimerHandle WaitHandle; // 1초 딜레이를 준 후 To do list 실행한다 for문의 설정횟수만큼 반복한다
	//WaitTime2 = 10.0f; // 연사 속도
	//GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString(TEXT("2 Sec Wait...")));
	//	}), WaitTime2, false); //반복도 여기서 추가 변수를 선언해 설정가능
}

void AWeaponRifle::Reload()
{
		CurrentMagazineAmmo = MagazineMaxAmmo;
}
