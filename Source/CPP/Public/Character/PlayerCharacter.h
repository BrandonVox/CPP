// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interface/PlayerInterface.h"
#include "PlayerCharacter.generated.h"

class UPlayerWidget;

class UEnhancedInputData;

UCLASS()
class CPP_API APlayerCharacter : 
	public ABaseCharacter, 
	public IPlayerInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*
	* Player Interface
	*/
	virtual void I_SetupEnemyStats(FText NameText, float Health, float MaxHealth) override;
	virtual void I_HandleEnemyHealthUpdated(float Health, float MaxHealth) override;


protected:
	virtual void BeginPlay() override;

	virtual void HandleTakePointDamage(AActor* DamagedActor, float Damage,
		class AController* InstigatedBy, FVector HitLocation,
		class UPrimitiveComponent* FHitComponent, FName BoneName,
		FVector ShotFromDirection, const class UDamageType* DamageType,
		AActor* DamageCauser) override;

private:
	void AddMapingContextForCharacter();

	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void AttackPressed();

private:
	UPROPERTY()
	UPlayerWidget* PlayerWidget;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Character Data Assets")
	UEnhancedInputData* EnhancedInputData;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> PlayerWidgetClass;


	
};
