// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/PlayerWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerWidget::UpdateHealthBar(float Health, float MaxHealth)
{
	if (HealthBar && HealthText_Player)
	{

		HealthBar->SetPercent(Health / MaxHealth);

		FString HealthString =
			FString::Printf(
				TEXT("%d / %d"),
				FMath::CeilToInt(Health),
				FMath::CeilToInt(MaxHealth)
			);

		HealthText_Player->SetText(FText::FromString(HealthString));

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

void UPlayerWidget::UpdateNameText_Enemy(FText GivenNameText)
{
	if(NameText_Enemy)

		NameText_Enemy->SetText(GivenNameText);
}

void UPlayerWidget::ShowEnemyStats()
{
	if(HealthBar_Enemy)
		HealthBar_Enemy->SetVisibility(ESlateVisibility::Visible);

	if(HealthText_Enemy)
		HealthText_Enemy->SetVisibility(ESlateVisibility::Visible);

	if(NameText_Enemy)
		NameText_Enemy->SetVisibility(ESlateVisibility::Visible);

}

void UPlayerWidget::HideEnemyStats()
{
	if (HealthBar_Enemy)
		HealthBar_Enemy->SetVisibility(ESlateVisibility::Hidden);

	if (HealthText_Enemy)
		HealthText_Enemy->SetVisibility(ESlateVisibility::Hidden);

	if (NameText_Enemy)
		NameText_Enemy->SetVisibility(ESlateVisibility::Hidden);
}
