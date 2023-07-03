#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttackInterface.generated.h"


UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UAttackInterface : public UInterface
{
	GENERATED_BODY()
};


class CPP_API IAttackInterface
{
	GENERATED_BODY()

public:
	virtual void I_AN_Combo() = 0;
	virtual FVector I_GetSocketLocation(const FName& SocketName) const = 0;
	virtual void I_ANS_TraceHit() = 0;
	virtual void I_ANS_BeginTraceHit() = 0;


	virtual void I_EnterCombat(AActor* TargetActor) = 0;
	virtual void I_ReceiveCombat(AActor* TargetActor) = 0;
	virtual void I_ExitCombat() = 0;
	virtual void I_HandleTargetExitCombat();

	virtual void I_HitTarget(float Health_Target, float MaxHealth_Target);
	virtual void I_AN_EndHitReact() = 0;

	virtual bool I_HasEnoughStamina(float Cost) const = 0;
	virtual void I_StaminaUpdated();
	virtual void I_StaminaUpdated_Target(float Stamina_Target, float MaxStamina_Target);
	virtual void I_RequestAttackFailed_Stamina(float StaminaCost);

	virtual void I_HandleTargetAttacked(float Stamina_Target, float MaxStamina_Target);
	virtual void I_HandleTargetDead();
	virtual void I_HandleTargetDestroyed();

	/*
	* Attack
	*/
	virtual bool I_IsAttacking() const = 0;
	virtual bool I_DoesReadyAttack() const = 0;
	UFUNCTION(BlueprintCallable)
	virtual void I_RequestAttack() = 0;
	virtual void I_PlayStartAttackSound() = 0;
	virtual void I_PlayAttackMontage(UAnimMontage* AttackMontage) = 0;
	virtual void I_HandleAttackSuccess() = 0;
	virtual void I_AN_EndAttack() = 0;
	/*
	* Stats
	*/
	virtual float I_GetHealth() const = 0;
	virtual float I_GetMaxHealth() const = 0;

	virtual float I_GetStamina() const = 0;
	virtual float I_GetMaxStamina() const = 0;
};