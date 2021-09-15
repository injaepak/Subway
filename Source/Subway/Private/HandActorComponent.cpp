// Fill out your copyright notice in the Description page of Project Settings.


#include "HandActorComponent.h"
#include "VR_Player.h"
#include "VRHandAnimInstance.h"
#include "Animation/AnimInstance.h"
// Sets default values for this component's properties
UHandActorComponent::UHandActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UHandActorComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AVR_Player>(GetOwner());

	if (player)
	{
		l_handAnim = Cast<UVRHandAnimInstance>(player->leftHand->GetAnimInstance());
		r_handAnim = Cast<UVRHandAnimInstance>(player->rightHand->GetAnimInstance());
	}
	
}


// Called every frame
void UHandActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	l_handAnim->gripValue = FMath::Lerp(l_handAnim->gripValue, targetGripValueLeft, gripSpeed * DeltaTime);
	r_handAnim->gripValue = FMath::Lerp(r_handAnim->gripValue, targetGripValueRight, gripSpeed * DeltaTime);
}

