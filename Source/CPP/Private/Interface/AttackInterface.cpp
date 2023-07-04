// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/AttackInterface.h"

// Add default functionality here for any IAttackInterface functions that are not pure virtual.

void IAttackInterface::I_HitTarget(float Health_Target, float MaxHealth_Target)
{
}

void IAttackInterface::I_HandleTargetDestroyed()
{
}

void IAttackInterface::I_HandleTargetAttacked(float Stamina_Target, float MaxStamina_Target)
{
}

void IAttackInterface::I_StaminaUpdated()
{
}

void IAttackInterface::I_StaminaUpdated_Target(float Stamina_Target, float MaxStamina_Target)
{
}

void IAttackInterface::I_RequestAttackFailed_Stamina(float StaminaCost)
{
}
