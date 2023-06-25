// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/EnemyCharacter.h"

#include "Interface/PlayerInterface.h"



#include "DataAsset/BaseCharacterData.h"

#include "Component/AttackComponent.h"
#include "Component/HealthComponent.h"
#include "Component/StaminaComponent.h"

#include "Controller/EnemyAIController.h"

#include "Kismet/KismetSystemLibrary.h" // debug
#include "Kismet/KismetMathLibrary.h"


AEnemyCharacter::AEnemyCharacter()
{
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	EnemyAIController = Cast<AEnemyAIController>(GetController());
}

void AEnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AEnemyCharacter::I_RequestAttack()
{
	if (AttackComponent == nullptr) return;

	AttackComponent->SetAttackType(GetDesireAttack_Type());
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

void AEnemyCharacter::I_AN_Combo()
{
	if (I_HasEnoughStamina(GetDesireAttack_Cost()) == false)
	{
		if (EnemyAIController)
			EnemyAIController->StartRegenMode();
		return;
	}

	Super::I_AN_Combo();
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

bool AEnemyCharacter::I_RegenEnoughStamina() const
{
	return I_HasEnoughStamina(GetDesireAttack_Cost());
}

void AEnemyCharacter::I_RotateToPlayer(AActor* PlayerActor)
{
	if (PlayerActor == nullptr) return;

	const auto DirectionToPlayer = 
		UKismetMathLibrary::GetDirectionUnitVector(
			GetActorLocation(),
			PlayerActor->GetActorLocation());

	const auto NewRotation = UKismetMathLibrary::MakeRotFromX(DirectionToPlayer);

	SetActorRotation(NewRotation);
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

EAttackType AEnemyCharacter::GetDesireAttack_Type() const
{
	if(AttackComponent == nullptr) 
		return EAttackType::Normal;

	if (AttackComponent->GetAttackCount_Normal() >= 4)
		return EAttackType::Strong;
	else
		return EAttackType::Normal;

	return EAttackType::Normal;
}

float AEnemyCharacter::GetDesireAttack_Cost() const
{
	if (BaseCharacterData == nullptr) return 0.0f;

	return BaseCharacterData->CostMap[GetDesireAttack_Type()];
}
