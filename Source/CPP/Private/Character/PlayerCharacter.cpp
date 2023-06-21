// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"

#include "PlayerController/BasePlayerController.h"

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	auto BasePlayerController = 
		Cast<ABasePlayerController>(GetController());
	
	if(BasePlayerController)
		BasePlayerController->HandleCharacterSpawned();
}
