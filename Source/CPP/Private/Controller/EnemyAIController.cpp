// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/EnemyAIController.h"

#include "Perception/AIPerceptionComponent.h"

#include "Perception/AISenseConfig_Sight.h"

#include "Kismet/KismetSystemLibrary.h" // debug

#include "Interface/EnemyInterface.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "Enum/EnemyAIState.h"

#include "Kismet/KismetMathLibrary.h"



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

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PossessedPawn = InPawn;

	EnemyInterface = TScriptInterface<IEnemyInterface>(InPawn);

	RunBehaviorTree(BehaviorTree);
	

	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated
			.AddDynamic(this, &AEnemyAIController::HandleTargetPerceptionUpdated);
	}

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

void AEnemyAIController::HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		HandleSeePlayer(Actor);
	}
}

void AEnemyAIController::CheckDistanceToPlayer(AActor* AIActor, AActor* PlayerActor,
	float AttackRange, float GiveUpRange)
{
	if (Blackboard == nullptr) return;

	if (AIActor == nullptr || PlayerActor == nullptr)
	{
		Blackboard->SetValueAsEnum(KeyEnemyAIState, (uint8)EEnemyAIState::Patrol);
		return;
	}

	if (bIsRegen) return;

	const float Distance_AI_Player = AIActor->GetDistanceTo(PlayerActor);

	if (Distance_AI_Player <= AttackRange)
		Blackboard->SetValueAsEnum(KeyEnemyAIState, (uint8)EEnemyAIState::Attack);

	else if (Distance_AI_Player > GiveUpRange)
		FightToPatrol();
		
	else
		Blackboard->SetValueAsEnum(KeyEnemyAIState, (uint8)EEnemyAIState::Fight);
}

void AEnemyAIController::UpdateRegenLocation(AActor* AIActor, AActor* PlayerActor, float RegenRange)
{
	if (PlayerActor == nullptr
		|| AIActor == nullptr
		|| Blackboard == nullptr) return;

	const auto Direction_Player_AI = UKismetMathLibrary::GetDirectionUnitVector(
		PlayerActor->GetActorLocation(),
		AIActor->GetActorLocation());

	const auto NewRegenLocation = PlayerActor->GetActorLocation() + (Direction_Player_AI * RegenRange);

	Blackboard->SetValueAsVector(Key_RegenLocation, NewRegenLocation);
}

void AEnemyAIController::CheckStamina()
{
	if (bIsRegen == false) return;
	
	if (EnemyInterface == nullptr) return;

	if (EnemyInterface->I_RegenEnoughStamina())
		bIsRegen = false;
}

void AEnemyAIController::StartRegenMode()
{
	bIsRegen = true;

	if(Blackboard)
		Blackboard->SetValueAsEnum(KeyEnemyAIState, (uint8)EEnemyAIState::Regen);
}

void AEnemyAIController::FightToPatrol()
{
	if (Blackboard == nullptr) return;

	Blackboard->SetValueAsEnum(KeyEnemyAIState, (uint8)EEnemyAIState::Patrol);
	DebugColor = FLinearColor::Gray;

	// Enemy Giveup
	if (EnemyInterface)
		EnemyInterface->I_FightToPatrol();
	//

	GetWorldTimerManager().SetTimer(
		GiveUpTimer,
		this,
		&AEnemyAIController::GiveUpTimerFinished,
		GiveUpSecond
	);
}

void AEnemyAIController::GiveUpTimerFinished()
{
	if (AIPerceptionComponent && AIPerceptionComponent->OnTargetPerceptionUpdated.IsBound() == false)
	{
		DebugColor = FLinearColor::Green;
		AIPerceptionComponent->OnTargetPerceptionUpdated
			.AddDynamic(this, &AEnemyAIController::HandleTargetPerceptionUpdated);
	}
}


void AEnemyAIController::HandleSeePlayer(AActor* Actor)
{
	DebugColor = FLinearColor::Red;

	// handle see player
	if (EnemyInterface)
		EnemyInterface->I_HandleSeePlayer(Actor);

	if (Blackboard)
	{
		Blackboard->SetValueAsEnum(KeyEnemyAIState, (uint8)EEnemyAIState::Fight);
		Blackboard->SetValueAsObject(KeyPlayerActor, Actor);
	}

	// Remove Delegate, Not Handle Event
	if(AIPerceptionComponent && AIPerceptionComponent->OnTargetPerceptionUpdated.IsBound())
		AIPerceptionComponent->OnTargetPerceptionUpdated
		.RemoveDynamic(this, &AEnemyAIController::HandleTargetPerceptionUpdated);
	


	// AIPerceptionComponent->OnTargetPerceptionUpdated.IsAlreadyBound()
}

void AEnemyAIController::HandleNotSeePlayer()
{
	DebugColor = FLinearColor::Green;

	if (Blackboard)
	{
		Blackboard->SetValueAsEnum(KeyEnemyAIState, (uint8)EEnemyAIState::Patrol);
		Blackboard->SetValueAsObject(KeyPlayerActor, nullptr);
	}
}