// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/PlayerWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerWidget::UpdateHealthBar_Player(float Health, float MaxHealth)
{
	if (HealthBar_Player && HealthText_Player)
	{

		HealthBar_Player->SetPercent(Health / MaxHealth);

		FString HealthString =
			FString::Printf(
				TEXT("%d / %d"),
				FMath::CeilToInt(Health),
				FMath::CeilToInt(MaxHealth)
			);

		HealthText_Player->SetText(FText::FromString(HealthString));

	}
}

void UPlayerWidget::UpdateStaminaBar_Player(float Stamina, float MaxStamina)
{
	if (StaminaBar_Player && StaminaText_Player)
	{

		StaminaBar_Player->SetPercent(Stamina / MaxStamina);

		FString StaminaString =
			FString::Printf(
				TEXT("%d / %d"),
				FMath::CeilToInt(Stamina),
				FMath::CeilToInt(MaxStamina)
			);

		StaminaText_Player->SetText(FText::FromString(StaminaString));

	}
}

void UPlayerWidget::UpdateHealthBar_Enemy(float Health, float MaxHealth)
{
	if (HealthBar_Enemy && HealthText_Enemy)
	{

		HealthBar_Enemy->SetPercent(Health / MaxHealth);

		FString HealthString =
			FString::Printf(
				TEXT("%d / %d"),
				FMath::CeilToInt(Health),
				FMath::CeilToInt(MaxHealth)
			);

		HealthText_Enemy->SetText(FText::FromString(HealthString));

	}
}

void UPlayerWidget::UpdateStaminaBar_Enemy(float Stamina, float MaxStamina)
{
	if (StaminaBar_Enemy && StaminaText_Enemy)
	{
		StaminaBar_Enemy->SetPercent(Stamina / MaxStamina);

		FString StaminaString =
			FString::Printf(
				TEXT("%d / %d"),
				FMath::CeilToInt(Stamina),
				FMath::CeilToInt(MaxStamina)
			);

		StaminaText_Enemy->SetText(FText::FromString(StaminaString));

	}
}

void UPlayerWidget::UpdateNameText_Enemy(FText GivenNameText)
{
	if(NameText_Enemy)

		NameText_Enemy->SetText(GivenNameText);
}

void UPlayerWidget::ShowEnemyStats()
{
	if (NameText_Enemy
		&& HealthBar_Enemy
		&& HealthText_Enemy
		&& StaminaBar_Enemy
		&& StaminaText_Enemy)
	{
		NameText_Enemy->SetVisibility(ESlateVisibility::Visible);
		HealthBar_Enemy->SetVisibility(ESlateVisibility::Visible);
		HealthText_Enemy->SetVisibility(ESlateVisibility::Visible);
		StaminaBar_Enemy->SetVisibility(ESlateVisibility::Visible);
		StaminaText_Enemy->SetVisibility(ESlateVisibility::Visible);
	}

}

void UPlayerWidget::HideEnemyStats()
{
	if (NameText_Enemy 
		&& HealthBar_Enemy 
		&& HealthText_Enemy 
		&& StaminaBar_Enemy
		&& StaminaText_Enemy)
	{
		NameText_Enemy->SetVisibility(ESlateVisibility::Hidden);
		HealthBar_Enemy->SetVisibility(ESlateVisibility::Hidden);
		HealthText_Enemy->SetVisibility(ESlateVisibility::Hidden);
		StaminaBar_Enemy->SetVisibility(ESlateVisibility::Hidden);
		StaminaText_Enemy->SetVisibility(ESlateVisibility::Hidden);
	}
}
