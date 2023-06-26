// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interface/EnemyInterface.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CPP_API AEnemyCharacter : 
	public ABaseCharacter,
	public IEnemyInterface
{
	GENERATED_BODY()
public:
	/*
	* Enemy Interface
	*/
	virtual FVector I_GetPatrolLocation() override;

private:
	UPROPERTY(EditInstanceOnly, Category = "Patrol")
	TArray<AActor*> PatrolPoints;

	int PatrolIndex = 0;
};
