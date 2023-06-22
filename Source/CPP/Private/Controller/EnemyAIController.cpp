// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/EnemyAIController.h"

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunBehaviorTree(BehaviorTree);

}
