// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Enum/AttackType.h"
#include "BaseCharacterData.generated.h"

class UAnimMontage;
class UParticleSystem;
class USoundBase;

UCLASS()
class CPP_API UBaseCharacterData : public UDataAsset
{
	GENERATED_BODY()
public:
	UBaseCharacterData();

public:
	/*
	* Attack
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TArray<UAnimMontage*> AttackMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	UAnimMontage* AttackMontage_Strong;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	USoundBase* StartAttackSound;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TMap<EAttackType, float> CostMap;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TMap<EAttackType, float> DamageMap;
	/*
	* Hit Impact
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Hit Impact")
	UParticleSystem* HitImpactEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Hit Impact")
	USoundBase* HitImpactSound;

	/*
	* Hitted
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Hitted")
	UAnimMontage* HitReactMontage_Front;

	UPROPERTY(EditDefaultsOnly, Category = "Hitted")
	UAnimMontage* HitReactMontage_Back;

	UPROPERTY(EditDefaultsOnly, Category = "Hitted")
	UAnimMontage* HitReactMontage_Right;

	UPROPERTY(EditDefaultsOnly, Category = "Hitted")
	UAnimMontage* HitReactMontage_Left;


	UPROPERTY(EditDefaultsOnly, Category = "Hitted")
	USoundBase* PainSound;
	/*
	* Dead
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Dead")
	UAnimMontage* DeadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Dead")
	USoundBase* DeadSound;
	/*
	* Trace Hit
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Trace Hit")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(EditDefaultsOnly, Category = "Trace Hit")
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(EditDefaultsOnly, Category = "Trace Hit")
	float TraceRadius = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Trace Hit")
	FName TraceStart;

	UPROPERTY(EditDefaultsOnly, Category = "Trace Hit")
	FName TraceEnd;

	UPROPERTY(EditDefaultsOnly, Category = "Trace Hit")
	float DrawTime = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Trace Hit")
	bool bDrawDebugTrace = true;

	/*
	* Stats
	*/

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	FText NameText;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float FightSpeed = 400.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float SprintSpeed = 900.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float Health = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float Stamina = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float MaxStamina = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float RegenSpeed_Stamina = 20.0f;
};
