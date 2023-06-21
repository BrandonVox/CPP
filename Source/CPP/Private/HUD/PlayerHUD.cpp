// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD.h"

#include "Blueprint/UserWidget.h"

void APlayerHUD::ShowPlayerWidget()
{

	APlayerController* PlayerController = GetOwningPlayerController();

	UUserWidget* PlayerWidget = CreateWidget<UUserWidget>(PlayerController, PlayerWidgetClass);

	if(PlayerWidget)
		PlayerWidget->AddToViewport();
}