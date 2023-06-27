// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BaseCharacterData.generated.h"

class UAnimMontage;
class UParticleSystem;
class USoundBase;

UCLASS()
class CPP_API UBaseCharacterData : public UDataAsset
{
	GENERATED_BODY()
public:
	// t array
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TArray< UAnimMontage*> AttackMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	UAnimMontage* AttackMontage_Strong;
	/*
	* Hit React
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Hit React")
	UAnimMontage* HitReactMontage_Front;

	UPROPERTY(EditDefaultsOnly, Category = "Hit React")
	UAnimMontage* HitReactMontage_Back;

	UPROPERTY(EditDefaultsOnly, Category = "Hit React")
	UAnimMontage* HitReactMontage_Right;

	UPROPERTY(EditDefaultsOnly, Category = "Hit React")
	UAnimMontage* HitReactMontage_Left;

	UPROPERTY(EditDefaultsOnly, Category = "Hit React")
	USoundBase* PainSound;
	/*
	* Dead
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Dead")
	UAnimMontage* DeadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Dead")
	USoundBase* DeadSound;
	/*
	* Hit Impact
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Hit Impact")
	UParticleSystem* HitImpactEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* HitImpactSound;



	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* StartAttackSound;
	

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
	float Damage = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float Health = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float Stamina = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float MaxStamina = 100.0f;
	/*
	* Speed
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Speed")
	float DefaultSpeed = 600.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Speed")
	float PatrolSpeed = 300.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Speed")
	float CombatSpeed = 400.0f;
};
