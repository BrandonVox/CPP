// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/EnemyCharacter.h"
#include "Kismet/KismetSystemLibrary.h" // debug
#include "Interface/PlayerInterface.h"



#include "DataAsset/BaseCharacterData.h"

#include "Component/AttackComponent.h"
#include "Component/HealthComponent.h"
#include "Component/StaminaComponent.h"


AEnemyCharacter::AEnemyCharacter()
{
}

void AEnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AEnemyCharacter::I_RequestAttack()
{
	if (AttackComponent == nullptr) return;

	if(AttackComponent->GetAttackCount_Normal() >= 4)
		AttackComponent->SetAttackType(EAttackType::Strong);
	else
		AttackComponent->SetAttackType(EAttackType::Normal);

	Super::I_RequestAttack();
}

void AEnemyCharacter::I_HandleAttackSuccess(float Cost)
{
	Super::I_HandleAttackSuccess(Cost);

	if (PlayerInterface)
		PlayerInterface->I_HandleEnemyStaminaUpdated(
			StaminaComponent->GetStamina(),
			StaminaComponent->GetMaxStamina()
		);
	
}

void AEnemyCharacter::I_HandleStaminaUpdated(float Stamina, float MaxStamina)
{
	Super::I_HandleStaminaUpdated(Stamina, MaxStamina);

	if(PlayerInterface)
		PlayerInterface->I_HandleEnemyStaminaUpdated(Stamina, MaxStamina);
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

	if (PlayerInterface && BaseCharacterData && HealthComponent && StaminaComponent)

		PlayerInterface->I_SetupEnemyStats(
			BaseCharacterData->NameText,
			HealthComponent->GetHealth(),
			HealthComponent->GetMaxHealth(),
			StaminaComponent->GetStamina(),
			StaminaComponent->GetMaxStamina()
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
