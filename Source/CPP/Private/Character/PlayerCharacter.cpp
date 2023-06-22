// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "PlayerController/BasePlayerController.h"
#include "Component/HealthComponent.h"



void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	auto BasePlayerController = 
		Cast<ABasePlayerController>(GetController());
	
	if(BasePlayerController && HealthComponent)
		BasePlayerController->
		HandleCharacterSpawned(HealthComponent->GetHealthPercent());
}
