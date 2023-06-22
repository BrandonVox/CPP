// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/EnemyAIController.h"

#include "Perception/AIPerceptionComponent.h"

#include "Perception/AISenseConfig_Sight.h"

#include "Kismet/KismetSystemLibrary.h" // debug

#include "Interface/EnemyInterface.h"

AEnemyAIController::AEnemyAIController()
{
	AIPerceptionComponent = 

		CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	
	AISightConfig = 

		CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI Sight Config"));

	AISightConfig->SightRadius = 2500.0f;
	AISightConfig->LoseSightRadius = 2500.0f;
	AISightConfig->PeripheralVisionAngleDegrees = 55.0f;
	AISightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerceptionComponent->ConfigureSense(*AISightConfig);
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (PossessedPawn == nullptr) return;

	UKismetSystemLibrary::DrawDebugCone(
		this,
		PossessedPawn->GetActorLocation(),
		PossessedPawn->GetActorForwardVector(),
		2500.0f,
		FMath::DegreesToRadians(55.0f),
		FMath::DegreesToRadians(55.0f),
		20,
		DebugColor,
		-1.0f,
		3.0f
	);

}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PossessedPawn = InPawn;

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

		DebugColor = FLinearColor::Red;

		// handle see player
		auto EnemyInterface = TScriptInterface<IEnemyInterface>(PossessedPawn);

		if (EnemyInterface)
			EnemyInterface->I_HandleSeePlayer(Actor);
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

		DebugColor = FLinearColor::Green;
	}
	
}
