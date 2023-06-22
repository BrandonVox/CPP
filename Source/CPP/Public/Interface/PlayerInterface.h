// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

class UBaseCharacterData;

class CPP_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void I_SetupEnemyStats(FText NameText, float Health, float MaxHealth) = 0;

	virtual void I_HandleEnemyHealthUpdated(float Health, float MaxHealth) = 0;
};
