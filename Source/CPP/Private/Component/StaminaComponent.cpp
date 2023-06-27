// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/StaminaComponent.h"
#include "DataAsset/BaseCharacterData.h"

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStaminaComponent::SetupComponent(UBaseCharacterData* BCD)
{
	if (BCD)
	{
		Stamina = BCD->Stamina;
		MaxStamina = BCD->MaxStamina;
	}
}

void UStaminaComponent::UpdateStamina(float Cost)
{
	Stamina = FMath::Max(Stamina - Cost, 0.0f);
}

