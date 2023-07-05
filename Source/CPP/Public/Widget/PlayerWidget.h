// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class CPP_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateHealthBar_Player(float Health, float MaxHealth);
	void UpdateHealthBar_Enemy(float Health, float MaxHealth);

	void UpdateStaminaBar_Player(float Stamina, float MaxStamina);
	void UpdateStaminaBar_Enemy(float Stamina, float MaxStamina);

	void ShowEnemyStats();
	void HideEnemyStats();

	void UpdateEliminations(int E);

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar_Player;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText_Player;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar_Player;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StaminaText_Player;


	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar_Enemy;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText_Enemy;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar_Enemy;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StaminaText_Enemy;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Eliminations;
};
