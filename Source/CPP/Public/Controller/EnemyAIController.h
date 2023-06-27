// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class IEnemyInterface;
class UAISenseConfig_Sight;

UCLASS()
class CPP_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void UpdatePatrolLocation();

	UFUNCTION(BlueprintCallable)
	void CheckDistance(AActor* AIActor, AActor* PlayerActor, float AttackRange);

protected:
	virtual void OnPossess(APawn* InPawn) override;
private:
	UFUNCTION()
	void HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	void HandleSeePlayer(AActor* Actor);
private:
	UPROPERTY()
	APawn* PossessedPawn;

	UPROPERTY(VisibleAnywhere)
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere)
	UAISenseConfig_Sight* AISightConfig;

	UPROPERTY()
	TScriptInterface<IEnemyInterface> EnemyInterface;

	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly)
	FName Key_PatrolLocation;

	UPROPERTY(EditDefaultsOnly)
	FName Key_PlayerActor;

	UPROPERTY(EditDefaultsOnly)
	FName Key_AIState;
	
	FLinearColor DebugColor = FLinearColor::Green;
};