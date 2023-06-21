// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/BasePlayerController.h"
#include "Blueprint/UserWidget.h"

void ABasePlayerController::HandleCharacterSpawned()
{

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1,
			1.0f,
			FColor::Cyan,
			TEXT("HandleCharacterSpawned()")
		);

	// Spawn Widget

	UUserWidget* PlayerWidget = 
		CreateWidget<UUserWidget>(this, PlayerWidgetClass);

	if (PlayerWidget)
		PlayerWidget->AddToViewport();
}
