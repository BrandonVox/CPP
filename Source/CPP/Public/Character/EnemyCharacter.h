// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interface/EnemyInterface.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CPP_API AEnemyCharacter : 
	public ABaseCharacter,
	public IEnemyInterface
{
	GENERATED_BODY()
public:
	virtual void Destroyed() override;

	// Attack Interface
	virtual void I_HandleAttackSuccess() override;

#pragma region Enemy_Interface
	virtual FVector I_GetPatrolLocation() override;
	virtual void I_HandleSeePlayer(AActor* PlayerActor) override;
#pragma endregion

	

protected:
	virtual void BeginPlay() override;

	virtual void HandleTakePointDamage(AActor* DamagedActor, float Damage,
		class AController* InstigatedBy, FVector HitLocation,
		class UPrimitiveComponent* FHitComponent, FName BoneName,
		FVector ShotFromDirection, const class UDamageType* DamageType,
		AActor* DamageCauser) override;

	virtual void HandleDead() override;

private:
	UFUNCTION()
	void HandlePlayerExitCombat();

private:
	UPROPERTY()
	TScriptInterface<IAttackInterface> AttackInterface_Player;

	UPROPERTY(EditInstanceOnly, Category = "Patrol")
	TArray<AActor*> PatrolPoints;

	int PatrolIndex = 0;
};
