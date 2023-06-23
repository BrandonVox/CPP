// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class UAISenseConfig_Sight;

UCLASS()
class CPP_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:

	UFUNCTION()
	void HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	void HandleSeePlayer(AActor* Actor);
	void HandleNotSeePlayer();

private:
	UPROPERTY()
	APawn* PossessedPawn;

	UPROPERTY(VisibleAnywhere)
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere)
	UAISenseConfig_Sight* AISightConfig;

	FLinearColor DebugColor = FLinearColor::Green;

	/*
	* Behavior Tree
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Behavior Tree")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "Behavior Tree")
	FName KeyIsFighting;

	UPROPERTY(EditDefaultsOnly, Category = "Behavior Tree")
	FName KeyPlayerActor;
	



};
