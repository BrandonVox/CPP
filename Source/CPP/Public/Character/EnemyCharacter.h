// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CPP_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AEnemyCharacter();

	UFUNCTION(BlueprintCallable)
	AActor* GetCorrectPatrolPoint();

private:
	UPROPERTY(EditInstanceOnly, Category = "Enemy AI")
	TArray<AActor*> PatrolPoints;

	int PatrolIndex = 0;
	
};
