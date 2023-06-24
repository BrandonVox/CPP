// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/StaminaComponent.h"
#include "DataAsset/BaseCharacterData.h"

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UStaminaComponent::SetupComponent(UBaseCharacterData* BCD)
{
	if (BCD == nullptr) return;

	Stamina = BCD->Stamina;
	MaxStamina = BCD->MaxStamina;
}


// Called when the game starts
void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

