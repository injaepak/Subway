// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HandActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUBWAY_API UHandActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHandActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	class UVRHandAnimInstance* l_handAnim;
	class UVRHandAnimInstance* r_handAnim;
	class AVR_Player* player;

	float targetGripValueLeft = 0;
	float targetGripValueRight = 0;

	UPROPERTY(EditAnywhere, Category = HandAnimSettings)
		float gripSpeed = 50.f;


};
