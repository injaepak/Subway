// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabActorComponent.h"
#include "PickUpActor.h"
#include "ShotGunActor.h"
#include "VR_Player.h"
#include "DrawDebugHelpers.h"
#include "HandActorComponent.h"
#include "Components/BoxComponent.h"
#include "WeaponPistol.h"
#include "WeaponShotgun.h"
#include "Subway.h"

// Sets default values for this component's properties
UGrabActorComponent::UGrabActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// �÷��̾ ĳ����
	player = Cast<AVR_Player>(GetOwner());

	player->handComp->targetGripValueRight = 0.0f;

	// �ӽ� �� ����
	bIsPistol = true;
	bIsShotgun = false;
}


// Called every frame
void UGrabActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsShowing)
	{
		DrawGrabLine();
	}
}

void UGrabActorComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	PlayerInputComponent->BindAction("RightGrip", IE_Pressed, this, &UGrabActorComponent::GrabAction);
	PlayerInputComponent->BindAction("RightGrip", IE_Released, this, &UGrabActorComponent::ReleaseAction);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &UGrabActorComponent::Fire);

	//PlayerInputComponent->BindAction("RightTrigger", IE_Pressed, this, &UGrabActorComponent::);
	//PlayerInputComponent->BindAction("RightTrigger", IE_Released, this, &UGrabActorComponent::);
}

void UGrabActorComponent::ShowGrabLine()
{
	bIsShowing = true;
}

void UGrabActorComponent::HideGrabLine()
{
	bIsShowing = false;
	grabObject = FHitResult();

}

void UGrabActorComponent::DrawGrabLine()
{
	// �տ� ��� ���ٸ�
	if (pickObject == nullptr)
	{
		FHitResult hitInfo;
		FVector startPos = player->rightHand->GetComponentLocation();
		FVector endPos = startPos + player->rightHand->GetForwardVector() * grabDistance;

		FCollisionObjectQueryParams objParams;
		objParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		objParams.AddObjectTypesToQuery(ECC_PhysicsBody);

		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(player);
		/*
				if (GetWorld()->LineTraceSingleByObjectType(hitInfo, startPos, endPos, objParams, queryParams))
				{
					DrawDebugLine(GetWorld(), startPos, hitInfo.ImpactPoint, FColor::Green, false, -1, 0, 2);
					grabObject = hitInfo;
				}
				else
				{
					DrawDebugLine(GetWorld(), startPos, endPos, FColor::Green, false, -1, 0, 2);;
					grabObject = FHitResult();
				}
		*/
		if (GetWorld()->SweepSingleByObjectType(hitInfo, startPos, startPos, FQuat::Identity, objParams, FCollisionShape::MakeSphere(15.f), queryParams))
		{
			grabObject = hitInfo;

			// ������ ��� �ִϸ��̼�
			//player->handComp->targetGripValueRight = 1.0f;
		}
		else
		{
			grabObject = FHitResult();
		}
		DrawDebugSphere(GetWorld(), startPos, 15.f, 30, FColor::Green, false, -1, 0, 1);
	}
}

void UGrabActorComponent::GrabAction()
{
	DrawGrabLine();

	AActor* grabActor = grabObject.GetActor();

	if (grabActor == nullptr)
	{
		return;
	}

	if (pickObject == nullptr)
	{
		FString gunName = grabActor->GetName();
		if(gunName.Contains("PickUpActor"))
		{
			pickObject = Cast<APickUpActor>(grabActor);
			pistol = Cast<AWeaponPistol>(pickObject->gun->GetChildActor());
	
			/*if (pistol)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, pistol->GetName());
			}*/

			if (pickObject)
			{
				//FAttachmentTransformRules attachRules = FAttachmentTransformRules::KeepWorldTransform;
				FAttachmentTransformRules attachRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;

				// �տ� ���̱�
				pickObject->boxComp->SetSimulatePhysics(false);
				pickObject->AttachToComponent(player->rightHand, attachRules, TEXT("GrabPoint"));

				// ������Ʈ�� ������� ��ġ ���
				pickObject->boxComp->SetRelativeLocation((pickObject->grabOffset));

				pickObject->boxComp->SetEnableGravity(false);

				// ������ ��� �ִϸ��̼�
				player->handComp->targetGripValueRight = 1.0f;
			}
		}
		else if (gunName.Contains("ShotgunActor"))
		{
			shotgunobject = Cast<AShotGunActor>(grabActor);
			shotgun = Cast<AWeaponShotgun>(shotgunobject->shotgun->GetChildActor());

			if (shotgunobject)
			{
				//FAttachmentTransformRules attachRules = FAttachmentTransformRules::KeepWorldTransform;
				FAttachmentTransformRules attachRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;

				// �տ� ���̱�
				shotgunobject->boxComp->SetSimulatePhysics(false);
				shotgunobject->AttachToComponent(player->rightHand, attachRules, TEXT("GrabPoint"));

				// ������Ʈ�� ������� ��ġ ���
				shotgunobject->boxComp->SetRelativeLocation((shotgunobject->grabOffset));

				shotgunobject->boxComp->SetEnableGravity(false);

				// ������ ��� �ִϸ��̼�
				player->handComp->targetGripValueRight = 1.0f;
			}
		}
	}
	else
	{
		return;
	}

}

void UGrabActorComponent::ReleaseAction()
{
	if (pickObject)
	{
		pickObject->boxComp->SetEnableGravity(true);
		// �� �ڸ����� ��������
		pickObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		pickObject->boxComp->SetSimulatePhysics(true);

		// ������ �Ǵ� �ִϸ��̼�
		player->handComp->targetGripValueRight = 0.0f;

		pickObject = nullptr;
	}

	else if (shotgunobject)
	{
		shotgunobject->boxComp->SetEnableGravity(true);
		// �� �ڸ����� ��������
		shotgunobject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		shotgunobject->boxComp->SetSimulatePhysics(true);

		// ������ �Ǵ� �ִϸ��̼�
		player->handComp->targetGripValueRight = 0.0f;

		shotgunobject = nullptr;
	}
}

void UGrabActorComponent::Test1()
{
	if (pickObject == nullptr)
	{
		FHitResult hitInfo;
		FVector startPos = player->rightHand->GetComponentLocation();

		FCollisionObjectQueryParams objParams;
		objParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		objParams.AddObjectTypesToQuery(ECC_PhysicsBody);

		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(player);
	}
}

void UGrabActorComponent::Test2()
{
	// ������ �Ǵ� �ִϸ��̼�
	player->handComp->targetGripValueRight = 0.0f;
}

void UGrabActorComponent::Fire()
{
	if (pickObject && pistol)
	{
		PRINTLOG(TEXT("ddddddddddd"));

		pistol->Fire();

		// ����ȿ��
		GetWorld()->GetFirstPlayerController()->PlayHapticEffect(shotHaptic, EControllerHand::Right, 1.f, false);

		// ���⿡ ���� �߰�
		
	}

	if (shotgunobject && shotgun)
	{
		PRINTLOG(TEXT("ddddddddddd"));

		shotgun->Fire();

		// ����ȿ��
		GetWorld()->GetFirstPlayerController()->PlayHapticEffect(shotHaptic, EControllerHand::Right, 10.f, false);
		GetWorld()->GetFirstPlayerController()->PlayHapticEffect(shotHaptic, EControllerHand::Left, 10.f, false);

		// ���⿡ ���� �߰�

	}
}

