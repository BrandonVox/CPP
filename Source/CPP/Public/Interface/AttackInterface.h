// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttackInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UAttackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */

class CPP_API IAttackInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void I_PlayAttackMontage(UAnimMontage* AttackMontage) = 0;
	virtual void I_PlayStartAttackSound() = 0;

	virtual void I_AN_EndAttack() = 0;
	virtual void I_AN_Combo() = 0;
	// getter
	virtual FVector I_GetSocketLocation(const FName& SocketName) const = 0;

	virtual void I_ANS_TraceHit() = 0;
	virtual void I_ANS_BeginTraceHit() = 0;

	// base 
	virtual void I_EnterCombat(AActor* TargetActor) = 0;
	virtual void I_ReceiveCombat(AActor* TargetActor) = 0;
	virtual void I_ExitCombat() = 0;

	virtual void I_HandleTargetExitCombat() = 0;

	virtual void I_HitTarget(float Health_Target, float MaxHealth_Target);

	UFUNCTION(BlueprintCallable)
	virtual void I_RequestAttack() = 0;

	virtual void I_AN_EndHitReact() = 0;

	virtual void I_HandleTargetDestroyed();

	virtual void I_HandleAttackSuccess() = 0;

	virtual void I_HandleTargetAttacked(float Stamina_Target, float MaxStamina_Target);

	virtual bool I_DoesReadyAttack() const = 0;

	virtual bool I_HasEnoughStamina(float Cost) const = 0;

	virtual void I_StaminaUpdated();

	virtual void I_StaminaUpdated_Target(float Stamina_Target, float MaxStamina_Target);

	virtual bool I_IsAttacking() const = 0;

	// base character
	virtual void I_RequestAttackFailed_Stamina(float StaminaCost);

	/*
	* Stats
	*/
	virtual float I_GetHealth() const = 0;
	virtual float I_GetMaxHealth() const = 0;

	virtual float I_GetStamina() const = 0;
	virtual float I_GetMaxStamina() const = 0;
};