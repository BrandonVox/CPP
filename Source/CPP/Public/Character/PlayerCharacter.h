// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UPlayerWidget;

UCLASS()
class CPP_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region Attack_Interface
	virtual void I_EnterCombat
		(float Health_Enemy, float MaxHealth_Enemy,
			float Stamina_Enemy, float MaxStamina_Enemy) override;
	virtual void I_HitTarget(float Health_Target, float MaxHealth_Target) override;
	virtual void I_HandleTargetDestroyed() override;

	virtual void I_HandleAttackSuccess() override;

	virtual void I_HandleTargetAttacked(float Stamina_Target, float MaxStamina_Target) override;
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
	void AddMapingContextForCharacter();

	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void AttackPressed();
	void StrongAttackPressed();
	void ExitCombatPressed();

private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> PlayerWidgetClass;

	UPROPERTY()
	UPlayerWidget* PlayerWidget;
};
