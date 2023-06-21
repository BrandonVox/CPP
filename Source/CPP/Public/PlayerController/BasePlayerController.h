// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CPP_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	void HandleCharacterSpawned(float HealthPercent);

private:

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> PlayerWidgetClass;

};
