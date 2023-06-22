// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"

AEnemyCharacter::AEnemyCharacter()
{
}

AActor* AEnemyCharacter::GetCorrectPatrolPoint()
{

	if (PatrolPoints.IsEmpty()) return nullptr;

	auto Result = PatrolPoints[PatrolIndex];

	PatrolIndex = (PatrolIndex + 1) % PatrolPoints.Num();

	return Result;
}
