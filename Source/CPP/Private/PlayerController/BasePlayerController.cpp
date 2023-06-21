// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/BasePlayerController.h"

#include "Widget/PlayerWidget.h"

void ABasePlayerController::HandleCharacterSpawned(float HealthPercent)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1,
			1.0f,
			FColor::Cyan,
			TEXT("HandleCharacterSpawned()")
		);

	// Spawn Widget

	UPlayerWidget* PlayerWidget =
		CreateWidget<UPlayerWidget>(this, PlayerWidgetClass);

	if (PlayerWidget == nullptr) return;

	PlayerWidget->UpdateHealthBar(HealthPercent);
	PlayerWidget->AddToViewport();
}
