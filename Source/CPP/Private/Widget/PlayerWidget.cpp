// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/PlayerWidget.h"

#include "Components/ProgressBar.h"

void UPlayerWidget::UpdateHealthBar(float NewPercent)
{
	if(HealthBar)
		HealthBar->SetPercent(NewPercent);
}
