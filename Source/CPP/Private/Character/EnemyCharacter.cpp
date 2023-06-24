// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/EnemyCharacter.h"
#include "Kismet/KismetSystemLibrary.h" // debug
#include "Interface/PlayerInterface.h"

#include "Component/HealthComponent.h"

#include "DataAsset/BaseCharacterData.h"

AEnemyCharacter::AEnemyCharacter()
{
}

void AEnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

AActor* AEnemyCharacter::I_GetCorrectPatrolPoint()
{
	if (PatrolPoints.IsEmpty()) return nullptr;

	auto Result = PatrolPoints[PatrolIndex];

	PatrolIndex = (PatrolIndex + 1) % PatrolPoints.Num();

	return Result;
}

void AEnemyCharacter::I_HandleSeePlayer(AActor* PlayerActor)
{
	// ten
	// mau
	// moi lan
	// player interface
	PlayerInterface = TScriptInterface<IPlayerInterface>(PlayerActor);

	if (PlayerInterface && BaseCharacterData && HealthComponent)

		PlayerInterface->I_SetupEnemyStats(
			BaseCharacterData->NameText,
			HealthComponent->GetHealth(),
			HealthComponent->GetMaxHealth()
		);
}

void AEnemyCharacter::I_FightToPatrol()
{
	if (PlayerInterface)
		PlayerInterface->I_ExitFight();
}

void AEnemyCharacter::HandleTakePointDamage(AActor* DamagedActor, float Damage,
	AController* InstigatedBy, FVector HitLocation,
	UPrimitiveComponent* FHitComponent, FName BoneName,
	FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	Super::HandleTakePointDamage(DamagedActor, Damage, InstigatedBy,
		HitLocation, FHitComponent, BoneName,
		ShotFromDirection, DamageType, DamageCauser);
	
	if(PlayerInterface && HealthComponent)

		PlayerInterface->I_HandleEnemyHealthUpdated(
				HealthComponent->GetHealth(),
				HealthComponent->GetMaxHealth()
			);
}

void AEnemyCharacter::HandleDead()
{
	Super::HandleDead();

	if (PlayerInterface)
		PlayerInterface->I_ExitFight();
	// remove controller
	DetachFromControllerPendingDestroy();
}
