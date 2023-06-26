// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/EnemyAIController.h"
#include "Interface/EnemyInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

// AI Module

AEnemyAIController::AEnemyAIController()
{
	// AI PERCEPTION COMPONENT
	AIPerceptionComponent = CreateDefaultSubobject
		<UAIPerceptionComponent>(TEXT("AI Perception Component"));

	// Sight Config
	AISightConfig = CreateDefaultSubobject
		<UAISenseConfig_Sight>(TEXT("AI Sight Config"));

	AISightConfig->SightRadius = 2500.0f;
	AISightConfig->LoseSightRadius = 2500.0f;
	AISightConfig->PeripheralVisionAngleDegrees = 55.0f;
	AISightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerceptionComponent->ConfigureSense(*AISightConfig);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess( InPawn);


	EnemyInterface = TScriptInterface<IEnemyInterface>(InPawn);

	// RunBehaviorTree(BehaviorTree);

	if(AIPerceptionComponent)
		AIPerceptionComponent->OnTargetPerceptionUpdated
			.AddDynamic(this, &AEnemyAIController::HandleTargetPerceptionUpdated);
}

void AEnemyAIController::HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(
				-1,
				1.0f,
				FColor::Green,
				TEXT("See Player")
			);

	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(
				-1,
				1.0f,
				FColor::Red,
				TEXT("Lose Sight Player")
			);
	}
}



void AEnemyAIController::UpdatePatrolLocation()
{
	if(Blackboard && EnemyInterface)
		Blackboard->SetValueAsVector(
			Key_PatrolLocation,
			EnemyInterface->I_GetPatrolLocation());
}