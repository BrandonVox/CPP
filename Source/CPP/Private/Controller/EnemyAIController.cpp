// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/EnemyAIController.h"
#include "Interface/EnemyInterface.h"
#include "BehaviorTree/BlackboardComponent.h"

// 



void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess( InPawn);


	EnemyInterface = TScriptInterface<IEnemyInterface>(InPawn);

	RunBehaviorTree(BehaviorTree);
}

void AEnemyAIController::UpdatePatrolLocation()
{
	if(Blackboard && EnemyInterface)
		Blackboard->SetValueAsVector(
			Key_PatrolLocation,
			EnemyInterface->I_GetPatrolLocation());
}