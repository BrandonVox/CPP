// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "Widget/PlayerWidget.h"
#include "Component/HealthComponent.h"

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	// player widget
	// world
	auto PlayerWidget =  
		CreateWidget<UPlayerWidget>(GetWorld(), PlayerWidgetClass);

	if (PlayerWidget && HealthComponent)
	{
		PlayerWidget->AddToViewport();
		// percent
		// health / max health
		PlayerWidget->
			UpdateHealthBar_Player(
				HealthComponent->Health / HealthComponent->MaxHealth);
	}
		
}
