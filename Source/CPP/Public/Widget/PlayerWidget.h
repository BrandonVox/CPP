// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

class UProgressBar;

UCLASS()
class CPP_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateHealthBar(float NewPercent);

private:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

};
