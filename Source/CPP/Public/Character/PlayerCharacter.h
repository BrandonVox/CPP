// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interface/PlayerInterface.h"
#include "PlayerCharacter.generated.h"

class UPlayerWidget;

UCLASS()
class CPP_API APlayerCharacter : 
	public ABaseCharacter, 
	public IPlayerInterface
{
	GENERATED_BODY()

public:
	/*
	* Player Interface
	*/
	virtual void I_SetupEnemyStats(FText NameText, float Health, float MaxHealth) override;
	virtual void I_HandleEnemyHealthUpdated(float Health, float MaxHealth) override;


protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> PlayerWidgetClass;

	UPROPERTY()
	UPlayerWidget* PlayerWidget;
	
};
