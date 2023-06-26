// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

class UPlayerWidget;

UCLASS()
class CPP_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	/*
	* Attack Interface
	*/
	virtual void I_EnterCombat(float Health_Enemy, float MaxHealth_Enemy) override;
	virtual void I_HitTarget(float Health_Target, float MaxHealth_Target) override;
protected:
	virtual void BeginPlay() override;
private:
	// begin play
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> PlayerWidgetClass;

	UPROPERTY()
	UPlayerWidget* PlayerWidget;
};
