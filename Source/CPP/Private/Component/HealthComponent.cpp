// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/HealthComponent.h"
#include "DataAsset/BaseCharacterData.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UHealthComponent::SetupComponent(UBaseCharacterData* BCD)
{
	if (BCD)
	{
		Health = BCD->Health;
		MaxHealth = BCD->MaxHealth;
	}
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

