// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/BaseAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UBaseAnimInstance::NativeInitializeAnimation()
{
	// Pawn -> Character
	Character = Cast<ACharacter>(TryGetPawnOwner());

	// nullptr
	if(Character)
		MovementComponent = Character->GetCharacterMovement();
}

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{

	if (MovementComponent == nullptr)
		return;
	
	GroundSpeed =  UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);

	bShouldMove = GroundSpeed > 0.0f;
}
