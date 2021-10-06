// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveActorComponent.h"
#include "VR_Player.h"
#include "MotionControllerComponent.h"
#include "Components/CapsuleComponent.h"
#include "HandActorComponent.h"

// Sets default values for this component's properties
UMoveActorComponent::UMoveActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMoveActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UE_LOG(LogTemp, Warning, TEXT("START!!!!!!!!!!!!!!"));

	player = Cast<AVR_Player>(GetOwner());
}


// Called every frame
void UMoveActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMoveActorComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("RightThumbstick_X", this, &UMoveActorComponent::MoveHorizontal);
	PlayerInputComponent->BindAxis("RightThumbstick_Y", this, &UMoveActorComponent::MoveVertical);
	PlayerInputComponent->BindAxis("LeftThumbstick_X", this, &UMoveActorComponent::RotateHorizontal);
	PlayerInputComponent->BindAction("LeftGrip", IE_Pressed, this, &UMoveActorComponent::GripOn);
	PlayerInputComponent->BindAction("LeftGrip", IE_Released, this, &UMoveActorComponent::GripOff);

	
}

void UMoveActorComponent::MoveHorizontal(float value)
{
}

void UMoveActorComponent::MoveVertical(float value)
{
}

void UMoveActorComponent::RotateHorizontal(float value)
{
}

void UMoveActorComponent::GripOn()
{
	

	gripCheck = true;

	 //손을 쥐는 애니메이션을 실행한다.
	/*UVRHandAnimInstance* handAnim = Cast<UVRHandAnimInstance>(player->leftHand->GetAnimInstance());

	if (handAnim)
	{
		handAnim->gripValue = 1.0f;
	}*/

	player->handComp->targetGripValueLeft = 0.f;

}

void UMoveActorComponent::GripOff()
{
	gripCheck = false;
	// 손을 펴는 애니메이션을 실행한다.
	player->handComp->targetGripValueLeft = 0;

	//손을 쥐는 애니메이션을 실행한다.
	/*UVRHandAnimInstance* handAnim = Cast<UVRHandAnimInstance>(player->leftHand->GetAnimInstance());

	if (handAnim)
	{
		handAnim->gripValue = 0.0f;
	}*/
}
