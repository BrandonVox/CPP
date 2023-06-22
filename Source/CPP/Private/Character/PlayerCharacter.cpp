// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "PlayerController/BasePlayerController.h"
#include "Component/HealthComponent.h"
#include "Widget/PlayerWidget.h"
#include "DataAsset/BaseCharacterData.h"

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Spawn Widget
	PlayerWidget =
		CreateWidget<UPlayerWidget>(GetWorld(), PlayerWidgetClass);

	if (PlayerWidget && HealthComponent)
	{

		PlayerWidget->AddToViewport();
		PlayerWidget->UpdateHealthBar(HealthComponent->GetHealthPercent());

		PlayerWidget->HideEnemyStats();
	}
}

void APlayerCharacter::I_SetupEnemyStats(FText NameText, float Health, float MaxHealth)
{
	if (PlayerWidget)
	{
		PlayerWidget->ShowEnemyStats();
		PlayerWidget->UpdateNameText_Enemy(NameText);
		PlayerWidget->UpdateHealthBar_Enemy(Health, MaxHealth);
	}

}

void APlayerCharacter::I_HandleEnemyHealthUpdated(float Health, float MaxHealth)
{

	if(PlayerWidget)
		PlayerWidget->UpdateHealthBar_Enemy(Health, MaxHealth);

}
