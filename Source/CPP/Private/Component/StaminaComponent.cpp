// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/StaminaComponent.h"
#include "DataAsset/BaseCharacterData.h"
#include "Interface/AttackInterface.h"


UStaminaComponent::UStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickInterval(0.015f);
}


void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	AttackInterface = TScriptInterface<IAttackInterface>(GetOwner());
}

void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	
	if (Stamina < MaxStamina && AttackInterface && AttackInterface->I_IsAttacking() == false)
	{
		Stamina = FMath::Min(Stamina + (RegenSpeed * DeltaTime), MaxStamina);

		AttackInterface->I_StaminaUpdated();
	}
}

void UStaminaComponent::SetupComponent(UBaseCharacterData* BCD)
{
	if (BCD)
	{
		Stamina = BCD->Stamina;
		MaxStamina = BCD->MaxStamina;
		RegenSpeed = BCD->RegenSpeed;
	}
}

void UStaminaComponent::UpdateStamina(float Cost)
{
	Stamina = FMath::Max(Stamina - Cost, 0.0f);
}

