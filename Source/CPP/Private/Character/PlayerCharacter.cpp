// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "Widget/PlayerWidget.h"
#include "Component/HealthComponent.h"
#include "DataAsset/BaseCharacterData.h"


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerWidget = CreateWidget<UPlayerWidget>(GetWorld(), PlayerWidgetClass);

	if (PlayerWidget && HealthComponent)
	{
		PlayerWidget->AddToViewport();
		PlayerWidget->
			UpdateHealthBar_Player(HealthComponent->Health, HealthComponent->MaxHealth);
		PlayerWidget->HideEnemyStats();

	}
		
}

void APlayerCharacter::I_EnterCombat(float Health_Enemy, float MaxHealth_Enemy)
{
	if (PlayerWidget)
	{
		PlayerWidget->ShowEnemyStats();
		PlayerWidget->UpdateHealthBar_Enemy(Health_Enemy, MaxHealth_Enemy);
	}

	if (BaseCharacterData)
		ChangeMaxWalkSpeed(BaseCharacterData->CombatSpeed);
		
}

void APlayerCharacter::I_HitTarget(float Health_Target, float MaxHealth_Target)
{
	// danh trung target
	// cap nhat mau
	// mau cua enemy
	if (PlayerWidget)
	{
		PlayerWidget->UpdateHealthBar_Enemy(Health_Target, MaxHealth_Target);
	}
}
