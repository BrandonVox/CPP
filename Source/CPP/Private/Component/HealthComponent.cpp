// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/HealthComponent.h"

#include "DataAsset/BaseCharacterData.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::SetupHealthComponent(UBaseCharacterData* BCD)
{
	if (BCD == nullptr) return;

	Health = BCD->Health;
	MaxHealth = BCD->MaxHealth;
}

float UHealthComponent::GetHealthPercent() const
{
	return Health / MaxHealth;
}

void UHealthComponent::UpdateHealthByDamage(float Damage)
{
	Health = FMath::Max(Health - Damage, 0.0f);
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

