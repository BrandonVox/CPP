// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"

// khi goi function nay
// thi minh se set gia tri cho key patrol location
// ai controller
// blackboard->set black board key
FVector AEnemyCharacter::I_GetPatrolLocation()
{
	if (PatrolPoints.IsEmpty()) return GetActorLocation();
	if (PatrolPoints[PatrolIndex] == nullptr) return GetActorLocation();
	
	auto ReturnValue = PatrolPoints[PatrolIndex]->GetActorLocation();

	PatrolIndex = (PatrolIndex + 1) % PatrolPoints.Num();

	return ReturnValue;
}
