// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UBaseCharacterData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPP_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void SetupHealthComponent(UBaseCharacterData* BCD);

	float GetHealthPercent() const;

	void UpdateHealthByDamage(float Damage);

protected:
	virtual void BeginPlay() override;
	
private:
	float Health = 0.0f;
	float MaxHealth = 0.0f;

public:

	FORCEINLINE
	float GetHealth() const { return Health; }

	FORCEINLINE
	float GetMaxHealth() const { return MaxHealth; }

};
