// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/BaseAnimInstance.h"
#include "Character/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enum/CombatState.h"

void UBaseAnimInstance::NativeInitializeAnimation()
{
	// Pawn -> Character
	Character = Cast<ABaseCharacter>(TryGetPawnOwner());

	// nullptr
	if(Character)
		MovementComponent = Character->GetCharacterMovement();
}

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{

	if (MovementComponent == nullptr || Character == nullptr)
		return;
	
	GroundSpeed =  UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);

	bShouldMove = GroundSpeed > 0.0f;

	bShouldBlendLowerUpper =
		GroundSpeed > 0.0f
		&& Character->GetCombatState() != ECombatState::Beaten;

	// Strafe

	const auto MovementRotation = UKismetMathLibrary::MakeRotFromX(Character->GetVelocity());

	const auto& AimRotation = Character->GetBaseAimRotation();
	
	StrafeValue = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1,
			1.0f,
			FColor::Blue,
			FString::Printf(TEXT("Value = %f"), StrafeValue)
		);

	bIsStrafing = Character->bIsStrafing;
}
