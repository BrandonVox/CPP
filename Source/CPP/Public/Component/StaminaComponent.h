// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

class UBaseCharacterData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPP_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaComponent();

	// virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetupComponent(UBaseCharacterData* BCD);

	void UpdateStaminaByCost(float Cost);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float Stamina = 0.0f;
	float MaxStamina = 0.0f;

public:
	FORCEINLINE
	float GetStamina() const { return Stamina; }

	FORCEINLINE
	float GetMaxStamina() const { return MaxStamina; }

};
