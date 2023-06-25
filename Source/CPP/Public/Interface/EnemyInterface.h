// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CPP_API IEnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void I_HandleSeePlayer(AActor* PlayerActor) = 0;
	virtual void I_FightToPatrol() = 0;

	UFUNCTION(BlueprintCallable)
	virtual AActor* I_GetCorrectPatrolPoint() = 0;

	virtual bool I_RegenEnoughStamina() const = 0;

};
