// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.generated.h"

class ABaseCharacter;
class UCharacterMovementComponent;

UCLASS()
class CPP_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bShouldMove = false;

	UPROPERTY(BlueprintReadOnly)
	bool bShouldBlendLowerUpper = true;

	UPROPERTY(BlueprintReadOnly)
	double StrafingValue = 0.0;

private:
	// basecharacter
	UPROPERTY()
	ABaseCharacter* Character;
	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;

	float GroundSpeed = 0.0f;
};
