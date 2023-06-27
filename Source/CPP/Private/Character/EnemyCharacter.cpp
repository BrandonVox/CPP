// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"
#include "Interface/AttackInterface.h"

#include "Component/HealthComponent.h"
#include "Component/StaminaComponent.h"
#include "Component/AttackComponent.h"

#include "DataAsset/BaseCharacterData.h"
#include "Controller/EnemyAIController.h"

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (BaseCharacterData)
		ChangeMaxWalkSpeed(BaseCharacterData->PatrolSpeed);

	EnemyAIController = Cast<AEnemyAIController>(GetController());
}

FVector AEnemyCharacter::I_GetPatrolLocation()
{
	if (PatrolPoints.IsEmpty()) return GetActorLocation();
	if (PatrolPoints[PatrolIndex] == nullptr) return GetActorLocation();
	
	auto ReturnValue = PatrolPoints[PatrolIndex]->GetActorLocation();

	PatrolIndex = (PatrolIndex + 1) % PatrolPoints.Num();

	return ReturnValue;
}

void AEnemyCharacter::I_HandleSeePlayer(AActor* PlayerActor)
{
	if (BaseCharacterData)
		ChangeMaxWalkSpeed(BaseCharacterData->CombatSpeed);

	AttackInterface_Player = TScriptInterface<IAttackInterface>(PlayerActor);

	if (AttackInterface_Player == nullptr) return;

	if (HealthComponent && StaminaComponent)
		AttackInterface_Player->I_EnterCombat(
			HealthComponent->Health,
			HealthComponent->MaxHealth,
			StaminaComponent->Stamina,
			StaminaComponent->MaxStamina
		);

	if(AttackInterface_Player->I_OnExitCombat.IsBound() == false)
		AttackInterface_Player->I_OnExitCombat.BindDynamic(this, &AEnemyCharacter::HandlePlayerExitCombat);
}

void AEnemyCharacter::Destroyed()
{
	if (AttackInterface_Player)
		AttackInterface_Player->I_HandleTargetDestroyed();

	Super::Destroyed();
}

void AEnemyCharacter::I_RequestAttack()
{
	if (AttackComponent == nullptr) return;

	if (AttackComponent->AttackCount_Normal >= 4)
	{
		AttackComponent->RequestAttackType = EAttackType::Strong;
	}
	else
	{
		AttackComponent->RequestAttackType = EAttackType::Normal;
	}

	// neu minh khong the tan cong
	// li do boi stamina khong du
	// lui lai de hoi phuc stamina
	Super::I_RequestAttack();
}

void AEnemyCharacter::I_HandleAttackSuccess()
{
	Super::I_HandleAttackSuccess();

	if(AttackInterface_Player && StaminaComponent)
		AttackInterface_Player->
			I_HandleTargetAttacked(StaminaComponent->Stamina, StaminaComponent->MaxStamina);
}

void AEnemyCharacter::I_StaminaUpdated()
{
	if(AttackInterface_Player && StaminaComponent)
		AttackInterface_Player->I_StaminaUpdated_Target
			(StaminaComponent->Stamina, StaminaComponent->MaxStamina);

	
	if (EnemyAIController == nullptr) return;

	if (EnemyAIController->bIsRegenStamina)
	{
		if (I_HasEnoughStamina(EnemyAIController->TargetStamina))
		{
			EnemyAIController->RegenToCombat();
		}
	}
}

void AEnemyCharacter::I_RequestAttackFailed_Stamina(float StaminaCost)
{
	if(EnemyAIController)
		EnemyAIController->StartRegenStamina(StaminaCost);
}

void AEnemyCharacter::HandleTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	Super::HandleTakePointDamage(DamagedActor, Damage, InstigatedBy,
		HitLocation, FHitComponent, BoneName, ShotFromDirection,
		DamageType, DamageCauser);
	// hit target
	// da danh trung enemy
	// cap nhat mau
	if (AttackInterface_Player && HealthComponent)
		AttackInterface_Player->
			I_HitTarget(HealthComponent->Health, HealthComponent->MaxHealth);
}

void AEnemyCharacter::HandleDead()
{
	Super::HandleDead();
	DetachFromControllerPendingDestroy();
}

void AEnemyCharacter::HandlePlayerExitCombat()
{
	if(EnemyAIController)
		EnemyAIController->BackToPatrol();
}