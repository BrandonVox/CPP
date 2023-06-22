// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"

#include "Kismet/KismetSystemLibrary.h" // debug

AEnemyCharacter::AEnemyCharacter()
{
}

void AEnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


}

AActor* AEnemyCharacter::GetCorrectPatrolPoint()
{

	if (PatrolPoints.IsEmpty()) return nullptr;

	auto Result = PatrolPoints[PatrolIndex];

	PatrolIndex = (PatrolIndex + 1) % PatrolPoints.Num();

	return Result;
}
