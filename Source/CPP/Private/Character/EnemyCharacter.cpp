// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"
#include "Interface/AttackInterface.h"
#include "Component/HealthComponent.h"

// khi goi function nay
// thi minh se set gia tri cho key patrol location
// ai controller
// blackboard->set black board key
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
	// attack interface
	// enter combat
	// health max health
	AttackInterface_Player = TScriptInterface<IAttackInterface>(PlayerActor);

	if(AttackInterface_Player && HealthComponent)
		AttackInterface_Player
			->I_EnterCombat(HealthComponent->Health, HealthComponent->MaxHealth);
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
