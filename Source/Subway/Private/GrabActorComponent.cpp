// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabActorComponent.h"
#include "PickUpActor.h"
#include "VR_Player.h"
#include "DrawDebugHelpers.h"
#include "HandActorComponent.h"
#include "Components/BoxComponent.h"

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

	player = Cast<AVR_Player>(GetOwner());

	player->handComp->targetGripValueRight = 0.0f;
	
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
		pickObject = Cast<APickUpActor>(grabActor);

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
