// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/EnemyAIController.h"
#include "Interface/EnemyInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "Enum/AIState.h"

// AI Module

AEnemyAIController::AEnemyAIController()
{
	SetGenericTeamId(GetGenericTeamId());

	// AI PERCEPTION COMPONENT
	AIPerceptionComponent = CreateDefaultSubobject
		<UAIPerceptionComponent>(TEXT("AI Perception Component"));

	// Sight Config
	AISightConfig = CreateDefaultSubobject
		<UAISenseConfig_Sight>(TEXT("AI Sight Config"));

	AISightConfig->SightRadius = 2500.0f;
	AISightConfig->LoseSightRadius = 2500.0f;
	AISightConfig->PeripheralVisionAngleDegrees = 55.0f;

	AISightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	AISightConfig->DetectionByAffiliation.bDetectNeutrals = false;
	AISightConfig->DetectionByAffiliation.bDetectEnemies = true;

	AIPerceptionComponent->ConfigureSense(*AISightConfig);
}

ETeamAttitude::Type AEnemyAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (APawn const* OtherPawn = Cast<APawn>(&Other))
	{
		if (auto const TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			if (TeamAgent->GetGenericTeamId() == GetGenericTeamId())
			{
				return ETeamAttitude::Friendly;
			}
			else
			{
				return ETeamAttitude::Hostile;
			}
		}
	}
	return ETeamAttitude::Neutral;
}

FGenericTeamId AEnemyAIController::GetGenericTeamId() const
{
	return TeamId;
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	PossessedPawn = InPawn;

	EnemyInterface = TScriptInterface<IEnemyInterface>(InPawn);

	RunBehaviorTree(BehaviorTree);

	if (AIPerceptionComponent && AIPerceptionComponent->OnTargetPerceptionUpdated.IsBound() == false)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic
		(this, &AEnemyAIController::HandleTargetPerceptionUpdated);
	}
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// draw debug cone
	// 
	// vi tri cua enemy
	// nhan vat ma ai controller dang possess
	// radian degree
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
	//if (Stimulus.WasSuccessfullySensed())
	//	DebugColor = FLinearColor::Red;
	//else
	//	DebugColor = FLinearColor::Green;

	//return;
	if (Stimulus.WasSuccessfullySensed())
	{
		HandleSeePlayer(Actor);
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

void AEnemyAIController::HandleSeePlayer(AActor* Actor)
{
	CombatMode(Actor);

	if (EnemyInterface)
		EnemyInterface->I_HandleSeePlayer(Actor);
}

void AEnemyAIController::CombatMode(AActor* Actor)
{
	DebugColor = FLinearColor::Red;



	if (Blackboard)
	{
		Blackboard->SetValueAsEnum(Key_AIState, (uint8)EAIState::Combat);
		Blackboard->SetValueAsObject(Key_PlayerActor, Actor);
	}


	if (AIPerceptionComponent && AIPerceptionComponent->OnTargetPerceptionUpdated.IsBound())
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.RemoveDynamic
		(this, &AEnemyAIController::HandleTargetPerceptionUpdated);
	}

	SetFocus(Actor);
}



void AEnemyAIController::UpdatePatrolLocation()
{
	if(Blackboard && EnemyInterface)
		Blackboard->SetValueAsVector(
			Key_PatrolLocation,
			EnemyInterface->I_GetPatrolLocation());
}

void AEnemyAIController::CheckDistance(AActor* AIActor, AActor* PlayerActor, float AttackRange)
{
	if (PlayerActor == nullptr) 
	{
		BackToPatrol();
		return;
	}

	if (bIsRegenStamina) return;

	if (AIActor == nullptr) return;

	const auto Distance_AI_Player = AIActor->GetDistanceTo(PlayerActor);

	if (Distance_AI_Player <= AttackRange)
	{
		if(Blackboard)
			Blackboard->SetValueAsEnum(Key_AIState, (uint8)EAIState::Attack);
	}
	else
	{
		if(Blackboard)
			Blackboard->SetValueAsEnum(Key_AIState, (uint8)EAIState::Combat);
	}
}

void AEnemyAIController::BackToPatrol()
{
	if (Blackboard)
		Blackboard->SetValueAsEnum(Key_AIState, (uint8)EAIState::Patrol);

	DebugColor = FLinearColor::Gray;

	if (GetWorldTimerManager().IsTimerActive(ExitCombatTimer))
		GetWorldTimerManager().ClearTimer(ExitCombatTimer);

	GetWorldTimerManager().SetTimer(
		ExitCombatTimer,
		this,
		&AEnemyAIController::ExitCombatTimerFinished,
		ExitCombatSecond
	);

	SetFocus(nullptr);
}

void AEnemyAIController::StartRegenStamina(float Stamina)
{
	bIsRegenStamina = true;
	TargetStamina = Stamina;

	if (Blackboard)
		Blackboard->SetValueAsEnum(Key_AIState, (uint8)EAIState::Regen);
}

void AEnemyAIController::RegenToCombat()
{
	bIsRegenStamina = false;

	if (Blackboard)
		Blackboard->SetValueAsEnum(Key_AIState, (uint8)EAIState::Combat);
}

void AEnemyAIController::UpdateRegenLocation(AActor* AIActor, AActor* PlayerActor, float RegenRange)
{
	if (PlayerActor == nullptr || AIActor == nullptr) return;

	const auto Direction_Player_AI = UKismetMathLibrary::GetDirectionUnitVector(
		PlayerActor->GetActorLocation(),
		AIActor->GetActorLocation());

	const auto RegenLocation = PlayerActor->GetActorLocation() + (Direction_Player_AI * RegenRange);

	if (Blackboard)
		Blackboard->SetValueAsVector(Key_RegenLocation, RegenLocation);

}

void AEnemyAIController::ExitCombatTimerFinished()
{
	DebugColor = FLinearColor::Green;

	if (AIPerceptionComponent && AIPerceptionComponent->OnTargetPerceptionUpdated.IsBound() == false)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic
			(this, &AEnemyAIController::HandleTargetPerceptionUpdated);
	}
}