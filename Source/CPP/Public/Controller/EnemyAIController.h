// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class UAISenseConfig_Sight;
class IEnemyInterface;

UCLASS()
class CPP_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void CheckDistanceToPlayer(AActor* AIActor, AActor* PlayerActor,
		float AttackRange, float GiveUpRange);

	UFUNCTION(BlueprintCallable)
	void UpdateRegenLocation(AActor* AIActor, AActor* PlayerActor, float RegenRange);

	UFUNCTION(BlueprintCallable)
	void CheckStamina();

	void StartRegenMode();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:

	UFUNCTION()
	void HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	void HandleSeePlayer(AActor* Actor);
	void HandleNotSeePlayer();

	void FightToPatrol();
	void GiveUpTimerFinished();

private:
	UPROPERTY()
	APawn* PossessedPawn;

	UPROPERTY()
	TScriptInterface<IEnemyInterface> EnemyInterface;

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
	FName KeyEnemyAIState;

	UPROPERTY(EditDefaultsOnly, Category = "Behavior Tree")
	FName KeyPlayerActor;

	UPROPERTY(EditDefaultsOnly, Category = "Behavior Tree")
	FName Key_RegenLocation;
	


	FTimerHandle GiveUpTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Behavior Tree")
	float GiveUpSecond = 3.0f;

	bool bIsRegen = false;

};
