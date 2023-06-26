// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/PlayerWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerWidget::UpdateHealthBar_Player(float Health, float MaxHealth)
{
	// progressbar
	if(HealthBar_Player)
		HealthBar_Player->SetPercent(Health / MaxHealth);
}

void UPlayerWidget::UpdateHealthBar_Enemy(float Health, float MaxHealth)
{
	if (HealthBar_Enemy)
		HealthBar_Enemy->SetPercent(Health / MaxHealth);

	FString HealthString = FString::Printf(TEXT("%d / %d"),
		FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));

	if(HealthText_Enemy)
		HealthText_Enemy->SetText(FText::FromString(HealthString));
}

void UPlayerWidget::ShowEnemyStats()
{
	if (HealthBar_Enemy && HealthText_Enemy)
	{
		HealthBar_Enemy->SetVisibility(ESlateVisibility::Visible);
		HealthText_Enemy->SetVisibility(ESlateVisibility::Visible);
	}

}

void UPlayerWidget::HideEnemyStats()
{
	if (HealthBar_Enemy && HealthText_Enemy)
	{
		HealthBar_Enemy->SetVisibility(ESlateVisibility::Hidden);
		HealthText_Enemy->SetVisibility(ESlateVisibility::Hidden);
	}
}

// enemy thay player
// enemy thong bao cho player
// player se hien dung chi so mau cua enemy