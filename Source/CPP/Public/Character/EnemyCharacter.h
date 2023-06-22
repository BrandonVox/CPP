// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interface/EnemyInterface.h"
#include "EnemyCharacter.generated.h"


class IPlayerInterface;

UCLASS()
class CPP_API AEnemyCharacter : 
	public ABaseCharacter, 
	public IEnemyInterface
{
	GENERATED_BODY()
public:
	AEnemyCharacter();

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	AActor* GetCorrectPatrolPoint();

	/*
	* Enemy Interface
	*/
	virtual void I_HandleSeePlayer(AActor* PlayerActor) override;

protected:
	virtual void HandleTakePointDamage(AActor* DamagedActor, float Damage,
		class AController* InstigatedBy, FVector HitLocation,
		class UPrimitiveComponent* FHitComponent, FName BoneName,
		FVector ShotFromDirection, const class UDamageType* DamageType,
		AActor* DamageCauser) override;

private:

	UPROPERTY()
	TScriptInterface<IPlayerInterface> PlayerInterface;

	UPROPERTY(EditInstanceOnly, Category = "Enemy AI")
	TArray<AActor*> PatrolPoints;

	int PatrolIndex = 0;
	
};
