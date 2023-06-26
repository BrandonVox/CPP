// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/PlayerWidget.h"
#include "Components/ProgressBar.h"

void UPlayerWidget::UpdateHealthBar_Player(float NewPercent)
{
	// progressbar
	if(HealthBar_Player)
		HealthBar_Player->SetPercent(NewPercent);
}
